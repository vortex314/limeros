#!/usr/bin/env python3
"""Summarise a motor-dashboard CSV log and suggest PID gains.

Usage:
    python3 tools/analyze_log.py motor-log.csv

The log is written by motor-dashboard (one row per received MotorEvent).
Hold each RPM target for at least ~10 s so the loop settles, then step to the
next target.  Steps between targets are what identify the plant gain.
"""

from __future__ import annotations

import csv
import math
import statistics as st
import sys


def load(path: str) -> list[dict]:
    rows: list[dict] = []
    with open(path, newline="") as handle:
        for raw in csv.DictReader(handle):
            try:
                row = {
                    "t": float(raw["epoch_ms"]) / 1000.0,
                    "rpm": float(raw["rpm_measured"]),
                    "target": float(raw["rpm_target"]),
                    "pwm": float(raw["pwm_percent"]),
                }
            except (KeyError, ValueError, TypeError):
                continue
            if any(math.isnan(value) for value in row.values()):
                continue
            rows.append(row)
    return rows


def split_segments(rows: list[dict], min_len: int = 10, tol: float = 1.0) -> list[list[dict]]:
    segments: list[list[dict]] = []
    current: list[dict] = []
    for row in rows:
        if current and abs(row["target"] - current[-1]["target"]) > tol:
            segments.append(current)
            current = []
        current.append(row)
    if current:
        segments.append(current)
    return [segment for segment in segments if len(segment) >= min_len]


def settled(segment: list[dict]) -> tuple[float, float, float]:
    tail = segment[len(segment) // 2:]
    rpm = st.mean(row["rpm"] for row in tail)
    ripple = st.pstdev([row["rpm"] for row in tail]) if len(tail) > 1 else 0.0
    pwm = st.mean(row["pwm"] for row in tail)
    return rpm, ripple, pwm


def zero_crossings(segment: list[dict]) -> tuple[int, float]:
    tail = segment[len(segment) // 2:]
    sign = 0
    crossings: list[float] = []
    for row in tail:
        error = row["target"] - row["rpm"]
        new_sign = 1 if error > 0 else -1 if error < 0 else 0
        if new_sign and sign and new_sign != sign:
            crossings.append(row["t"])
        if new_sign:
            sign = new_sign
    if len(crossings) < 2:
        return len(crossings), 0.0
    period = (crossings[-1] - crossings[0]) / (len(crossings) - 1)
    return len(crossings), period


def main() -> int:
    path = sys.argv[1] if len(sys.argv) > 1 else "motor-log.csv"
    rows = load(path)
    if not rows:
        print(f"no usable samples in {path}")
        return 1

    duration = rows[-1]["t"] - rows[0]["t"]
    print(f"{len(rows)} samples over {duration:.1f} s from {path}\n")

    segments = split_segments(rows)
    if not segments:
        print("no settled target segments found (hold a target for >3 s)")
        return 1

    print(f"{'target':>8} {'mean rpm':>9} {'ripple':>8} {'pwm %':>8} {'error':>8} {'crossings':>10}")
    for segment in segments:
        rpm, ripple, pwm = settled(segment)
        target = st.mean(row["target"] for row in segment)
        crossings, period = zero_crossings(segment)
        period_text = f"{period:.2f}s" if period else "-"
        print(
            f"{target:8.0f} {rpm:9.1f} {ripple:8.1f} {pwm:8.2f} "
            f"{target - rpm:8.1f} {crossings:6d} {period_text:>7}"
        )

    gains: list[float] = []
    for before, after in zip(segments, segments[1:]):
        _, _, pwm_before = settled(before)
        _, _, pwm_after = settled(after)
        rpm_before, _, _ = settled(before)
        rpm_after, _, _ = settled(after)
        if abs(pwm_after - pwm_before) >= 1.0:
            gains.append((rpm_after - rpm_before) / (pwm_after - pwm_before))

    print()
    if not gains:
        print("Could not identify the plant gain: step the target between at least")
        print("two settled speeds (e.g. 500 -> 1000 -> 1500 RPM, 10 s each).")
        return 0

    plant = st.median(gains)
    print(f"plant gain       ~{plant:.2f} RPM per % duty")
    if plant <= 0:
        print("plant gain is not positive - check the direction of the PWM")
        return 0

    kp = 0.5 / plant
    ti = 0.3
    print(f"suggested gains  Kp={kp:.4f}  Ki={kp / ti:.4f}  Kd=0")
    print(f"                 (loop gain 0.5, integral time {ti:.1f} s)")
    print("                 upload with: dashboard PID target fields, or firmware defaults")

    saturated = [segment for segment in segments if settled(segment)[2] > 97.0]
    if saturated:
        print("\nwarning: PWM saturates near 100 % for some targets - the motor")
        print("         cannot reach those speeds, so gains cannot fix the error.")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
