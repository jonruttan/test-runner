#!/usr/bin/env sh

#
# test-runner-coverage.sh
#
# Build and run tests with coverage enabled, then print a gcovr summary.
#
# Usage:
#   sh ./test-runner-coverage.sh [tests|<glob>|<file1> <file2> ...]
#
# Environment:
#   OUTDIR  Output directory for artifacts (default: .coverage)
#   CC      C compiler (default: cc)
#   CFLAGS  Compile flags (default: coverage-friendly flags + -Iinclude)
#   GCOVR   gcovr binary name/path (default: gcovr)
#

set -eu

SCRIPT="$0"
ROOT="$(CDPATH= cd -- "$(dirname -- "$SCRIPT")" && pwd)"

CORE="$ROOT/test-runner-core.sh"
OUTDIR="${OUTDIR:-$ROOT/.coverage}"
CC="${CC:-cc}"
GCOVR="${GCOVR:-gcovr}"
GCOVR_FLAGS="${GCOVR_FLAGS:---print-summary}"

INCLUDE="$ROOT/include"
if [ -z "${CFLAGS:-}" ]; then
	CFLAGS="-std=c11 -Wall -Wextra -Wno-unused-parameter -O0 -g --coverage -I$INCLUDE"
else
	# Ensure include/ is on the path when callers override CFLAGS.
	CFLAGS="$CFLAGS -I$INCLUDE"
fi

if [ ! -f "$CORE" ]; then
	echo "ERROR: missing core runner: $CORE" >&2
	exit 1
fi

command -v "$GCOVR" >/dev/null 2>&1 || {
	echo "ERROR: gcovr not found (install: python3 -m pip install --user gcovr)" >&2
	exit 1
}

rm -rf -- "$OUTDIR"
mkdir -p -- "$OUTDIR"

OUTDIR="$OUTDIR" CC="$CC" CFLAGS="$CFLAGS" \
	sh "$CORE" "$@"

"$GCOVR" -r "$ROOT" --object-directory "$OUTDIR" --filter 'include/' $GCOVR_FLAGS
