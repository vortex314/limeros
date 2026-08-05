#!/usr/bin/env bash
#
# generate_mavlink.sh - generate MAVLink v2 code from hcl/robot.xml
# using mavgen (from pymavlink).
#
# Usage:
#   ./hcl/generate_mavlink.sh [lang]
#
#   lang    c      -> MAVLink C library         (default)
#           c++11  -> single C++11 header
#           python -> single Python module
#
# Output (relative to repo root):
#   c      -> generated/mavlink/c/mavlink.h  + generated/mavlink/c/mavlink/robot/
#   c++11  -> generated/mavlink/c++11/       (C++11 header tree)
#   python -> generated/mavlink/python/robot.py
#
# On first run a local virtualenv is created at hcl/.venv-mavlink and
# pymavlink is installed there (no system-wide changes required).

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
DIALECT="$SCRIPT_DIR/robot.xml"

LANG_ARG="${1:-c}"

VENV_DIR="$SCRIPT_DIR/.venv-mavlink"
VENV_PY="$VENV_DIR/bin/python"

OUT_BASE="$ROOT_DIR/generated/mavlink"

# --- 1. virtualenv + pymavlink on first run --------------------------------
if [[ ! -x "$VENV_PY" ]]; then
    echo "[mavgen] first run: creating virtualenv at $VENV_DIR"
    python3 -m venv "$VENV_DIR"
    "$VENV_DIR/bin/pip" install --upgrade pip >/dev/null
    "$VENV_DIR/bin/pip" install pymavlink
fi

# --- 2. map language argument to mavgen options ----------------------------
case "$LANG_ARG" in
    c)
        MAV_LANG="C"
        TARGET="$OUT_BASE/c"
        ;;
    c++11)
        MAV_LANG="C++11"
        TARGET="$OUT_BASE/c++11"
        ;;
    python)
        MAV_LANG="Python"
        TARGET="$OUT_BASE/python/robot.py"
        ;;
    *)
        echo "error: unknown language '$LANG_ARG' (use: c, c++11, python)" >&2
        exit 2
        ;;
esac

mkdir -p "$(dirname "$TARGET")"

# --- 3. run mavgen (MAVLink v2 wire protocol) ------------------------------
echo "[mavgen] dialect: $DIALECT"
echo "[mavgen] lang   : $MAV_LANG"
echo "[mavgen] output : $TARGET"
"$VENV_DIR/bin/mavgen.py" \
    --lang="$MAV_LANG" \
    --wire-protocol=2.0 \
    --output="$TARGET" \
    "$DIALECT"

echo "[mavgen] done."
