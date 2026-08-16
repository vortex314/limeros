# PlatformIO extra_scripts: patch Energia.h so the project can build with C++17.
#
# The Energia framework (framework-energiativa) was written for GCC 4.8 and
# unconditionally defines function-like min()/max() macros in Energia.h:
#
#     #define min(a,b) ((a)<(b)?(a):(b))
#     #define max(a,b) ((a)>(b)?(a):(b))
#
# Modern libstdc++ (GCC >= 7) calls std::min/std::max internally in headers
# such as <bits/basic_string.tcc>, so these macros expand inside the standard
# library and produce "expected unqualified-id before '(' token" errors.
#
# This script restricts the macros to C mode (#ifndef __cplusplus). C++ code
# should use std::min/std::max from <algorithm> instead. The Energia core and
# this project's sources do not use the bare macros, so this is safe.
#
# The patch is idempotent and applied to the installed package on every build,
# so it survives package re-installs automatically.
import os

Import("env")  # noqa: F821 - provided by PlatformIO/SCons


def _patch_energia_h():
    pkg_dir = os.path.expanduser("~/.platformio/packages/framework-energiativa")
    energia_h = os.path.join(pkg_dir, "cores", "tivac", "Energia.h")

    if not os.path.isfile(energia_h):
        print("[patch_energia_cpp17] Energia.h not found at %s - skipping" % energia_h)
        return

    with open(energia_h, "r", encoding="utf-8") as f:
        content = f.read()

    marker = "// [patch_energia_cpp17] min/max macros only in C mode"
    if marker in content:
        print("[patch_energia_cpp17] Energia.h already patched - skipping")
        return

    old_block = (
        "#define min(a,b) ((a)<(b)?(a):(b))\n"
        "#define max(a,b) ((a)>(b)?(a):(b))"
    )
    new_block = (
        marker
        + "\n"
        + "// These macros break libstdc++ (std::min/std::max) under C++17/GCC>=7,\n"
        + "// so they are restricted to C (non-__cplusplus) builds. In C++ use\n"
        + "// std::min/std::max from <algorithm> instead.\n"
        + "#ifndef __cplusplus\n"
        + old_block
        + "\n"
        + "#endif"
    )

    if old_block not in content:
        print("[patch_energia_cpp17] min/max block not found in Energia.h - skipping")
        return

    content = content.replace(old_block, new_block, 1)

    with open(energia_h, "w", encoding="utf-8") as f:
        f.write(content)

    print("[patch_energia_cpp17] Patched Energia.h (min/max macros restricted to C mode)")


def _patch_itoa():
    """Fix utoa() conflicting declaration with newlib's stdlib.h.

    Energia's itoa.h/itoa.c declare utoa(unsigned long, char*, int), but the
    GCC >= 9 newlib stdlib.h already declares utoa(unsigned, char*, int).
    On this 32-bit target unsigned long and unsigned are distinct C types, so
    the redeclaration is a hard error. Align Energia's signature with newlib.
    """
    core_dir = os.path.expanduser("~/.platformio/packages/framework-energiativa/cores/tivac")
    files = ("itoa.h", "itoa.c")
    for name in files:
        path = os.path.join(core_dir, name)
        if not os.path.isfile(path):
            print("[patch_energia_cpp17] %s not found - skipping" % path)
            continue
        with open(path, "r", encoding="utf-8") as f:
            content = f.read()
        if "unsigned long" not in content:
            continue
        old_decl = "utoa( unsigned long value, char *string, int radix )"
        new_decl = "utoa( unsigned value, char *string, int radix )"
        if old_decl not in content:
            print("[patch_energia_cpp17] %s: utoa declaration not found - skipping" % name)
            continue
        content = content.replace(old_decl, new_decl, 1)
        with open(path, "w", encoding="utf-8") as f:
            f.write(content)
        print("[patch_energia_cpp17] Patched %s (utoa signature aligned with newlib)" % name)


_patch_energia_h()
_patch_itoa()
