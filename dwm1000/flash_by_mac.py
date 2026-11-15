#!/usr/bin/env python3
"""Flash every connected board with the PlatformIO env that belongs to it.

USB port numbers (/dev/ttyUSBn) change between plug-ins, so the port alone
does not say which board is which. This script reads the ESP chip MAC on each
port (esptool read_mac: one reset per board, harmless), looks the MAC up in the
`custom_mac` option of the envs in PROJECTS, and runs

    pio run -e <env> -t upload --upload-port <port>

in that env's project directory. A port whose MAC matches no env is refused,
and so is an env that matches more than one port. Nothing is flashed until all
MACs have been read.

Usage:
    ./flash_by_mac.py                    # flash every recognised board
    ./flash_by_mac.py --dry-run          # only show port -> MAC -> env
    ./flash_by_mac.py anchor2 anchor5    # flash only these envs
    ./flash_by_mac.py --ports /dev/ttyUSB0 /dev/ttyUSB3

Exit status is non-zero if any port was refused or any upload failed.
"""

import argparse
import configparser
import glob
import os
import re
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
# PlatformIO projects whose envs may carry a `custom_mac = xx:xx:xx:xx:xx:xx`.
PROJECTS = ["anchor", "tag_esp32"]

PIO_HOME = os.path.expanduser("~/.platformio")
PIO_PYTHON = os.path.join(PIO_HOME, "penv", "bin", "python")
ESPTOOL = os.path.join(PIO_HOME, "packages", "tool-esptoolpy", "esptool.py")


def norm_mac(mac):
    return mac.strip().lower().replace("-", ":")


def load_env_table():
    """MAC -> (project dir, env name), from custom_mac in each platformio.ini."""
    table = {}
    for proj in PROJECTS:
        ini = os.path.join(HERE, proj, "platformio.ini")
        if not os.path.exists(ini):
            continue
        cp = configparser.RawConfigParser()  # no ${...} interpolation
        cp.read(ini)
        for section in cp.sections():
            if not section.startswith("env:") or not cp.has_option(section, "custom_mac"):
                continue
            mac = norm_mac(cp.get(section, "custom_mac"))
            env = section[len("env:"):]
            if mac in table:
                sys.exit(f"error: MAC {mac} is listed for both {table[mac][1]} and {env}")
            table[mac] = (os.path.join(HERE, proj), env)
    return table


def find_pio():
    pio = shutil.which("pio")
    if pio:
        return pio
    pio = os.path.join(PIO_HOME, "penv", "bin", "pio")
    if os.path.exists(pio):
        return pio
    sys.exit("error: pio not found (install PlatformIO Core or add it to PATH)")


def read_mac(port):
    """Chip MAC of the ESP on `port`, or None (error text printed)."""
    python = PIO_PYTHON if os.path.exists(PIO_PYTHON) else sys.executable
    cmd = [python, ESPTOOL, "--port", port, "--after", "hard_reset", "read_mac"]
    try:
        out = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
    except subprocess.TimeoutExpired:
        print(f"  {port}: read_mac timed out")
        return None
    m = re.search(r"MAC:\s*([0-9a-fA-F:]{17})", out.stdout)
    if not m:
        last = (out.stdout + out.stderr).strip().splitlines()[-1:] or ["no output"]
        print(f"  {port}: read_mac failed: {last[0]}")
        return None
    chip = re.search(r"Chip is (\S+)", out.stdout)
    return norm_mac(m.group(1)), (chip.group(1) if chip else "?")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("envs", nargs="*", help="only flash these envs (default: all recognised)")
    ap.add_argument("--ports", nargs="+", help="ports to check (default: /dev/ttyUSB* /dev/ttyACM*)")
    ap.add_argument("--dry-run", action="store_true", help="read MACs and show the plan, flash nothing")
    args = ap.parse_args()
    sys.stdout.reconfigure(line_buffering=True)  # keep our lines in order with pio's output

    if not os.path.exists(ESPTOOL):
        sys.exit(f"error: esptool not found at {ESPTOOL}")
    table = load_env_table()
    known_envs = {env for _, env in table.values()}
    for e in args.envs:
        if e not in known_envs:
            sys.exit(f"error: unknown env '{e}' (known: {', '.join(sorted(known_envs))})")

    ports = args.ports or sorted(glob.glob("/dev/ttyUSB*") + glob.glob("/dev/ttyACM*"))
    if not ports:
        sys.exit("error: no serial ports found")

    print("Reading MACs (resets each board):")
    plan, refused = [], []          # plan: (port, mac, projdir, env)
    for port in ports:
        r = read_mac(port)
        if r is None:
            refused.append((port, "MAC not readable"))
            continue
        mac, chip = r
        if mac not in table:
            print(f"  {port}: {mac} ({chip})  -> UNKNOWN, refusing to flash")
            refused.append((port, f"unknown MAC {mac}"))
            continue
        projdir, env = table[mac]
        print(f"  {port}: {mac} ({chip})  -> {env}")
        plan.append((port, mac, projdir, env))

    # an env must not appear on two ports (e.g. a cloned MAC or a stale table)
    seen = {}
    for port, _, _, env in plan:
        seen.setdefault(env, []).append(port)
    dup = {env: p for env, p in seen.items() if len(p) > 1}
    for env, p in dup.items():
        print(f"  {env} matches several ports {p}: refusing all of them")
        refused += [(x, f"{env} on several ports") for x in p]
    plan = [x for x in plan if x[3] not in dup]

    if args.envs:
        plan = [x for x in plan if x[3] in args.envs]
        missing = set(args.envs) - {x[3] for x in plan}
        for e in sorted(missing):
            print(f"  {e}: no connected board found")
            refused.append(("-", f"{e} not connected"))

    if args.dry_run:
        print("\nDry run, would flash:")
        for port, _, projdir, env in plan:
            print(f"  {env:12s} {port}  ({os.path.relpath(projdir, HERE)})")
        return 1 if refused else 0

    pio = find_pio()
    failed = []
    for port, _, projdir, env in plan:
        print(f"\n=== {env} -> {port} ({os.path.relpath(projdir, HERE)})")
        rc = subprocess.run([pio, "run", "-e", env, "-t", "upload", "--upload-port", port], cwd=projdir).returncode
        if rc != 0:
            failed.append((env, port))

    print("\nSummary:")
    for port, _, _, env in plan:
        status = "FAILED" if (env, port) in failed else "ok"
        print(f"  {env:12s} {port:14s} {status}")
    for port, why in refused:
        print(f"  {'refused':12s} {port:14s} {why}")
    return 1 if (failed or refused) else 0


if __name__ == "__main__":
    sys.exit(main())
