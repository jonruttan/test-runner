#!/bin/sh

#
# test-runner-coverage.sh
#
# Configure coverage flags and run gcovr via test-runner's ANALYZER hook.
#
# Usage (from project root):
#   sh tests/c/test-runner/test-runner-coverage.sh [tests|<glob>|<file1> ...]
#
# Environment variables:
#   ANALYZER       - coverage tool (default: gcovr)
#   ANALYZER_FLAGS - extra flags (default: --print-summary)
#   CFLAGS         - compiler flags (coverage flags are appended)
#   COVERAGE_DIR   - HTML report output directory (default: .coverage)
#
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ANALYZER="${ANALYZER:-gcovr}"
ANALYZER_FLAGS="${ANALYZER_FLAGS:---print-summary}"
COVERAGE_DIR="${COVERAGE_DIR:-.coverage}"

command -v "$ANALYZER" >/dev/null 2>&1 || {
	echo "ERROR: $ANALYZER not found — install with: pip install gcovr" >&2
	exit 1
}

if [ "${ANALYZER:-}" = 'gcovr' ]; then
	ANALYZER="$ANALYZER -r . --object-directory \"\$1\" --filter 'src/' $ANALYZER_FLAGS --html-details $COVERAGE_DIR/index.html"
fi

# Ensure coverage output directory exists
mkdir -p "$COVERAGE_DIR"

# Append coverage instrumentation flags to whatever CFLAGS are set
CFLAGS="${CFLAGS} -O0 --coverage"

CFLAGS="$CFLAGS" ANALYZER="$ANALYZER" sh "$SCRIPT_DIR/test-runner.sh" "$@"
