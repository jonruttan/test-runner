#!/usr/bin/env sh

#
# test-runner-coverage.sh
#
# Configure coverage flags and run gcovr via test-runner's ANALYZER hook.
#
# Usage:
#   sh ./test-runner-coverage.sh [tests|<glob>|<file1> <file2> ...]
#
ANALYZER="${ANALYZER:-gcovr}"
ANALYZER_FLAGS="${ANALYZER_FLAGS:---print-summary}"

command -v "$ANALYZER" >/dev/null 2>&1 || {
	echo "ERROR: $ANALYZER not found" >&2
	exit 1
}

[ "${ANALYZER:-}" = 'gcovr' ] && ANALYZER="$ANALYZER -r . --object-directory \"\$1\" --filter \"include/\" $GCOVR_FLAGS"

# Let test-runner.sh add its usual DEBUG/TESTS flags and -Iinclude; we add coverage flags.
CFLAGS="${CFLAGS:-"-std=c11 -Wall -Wextra -Wno-unused-parameter -O0 -g --coverage"}"

# OUTDIR is created by test-runner.sh. We want gcovr to see that resolved path, so
# ANALYZER is evaluated in test-runner.sh and OUTDIR is passed as $1.
CFLAGS="$CFLAGS" ANALYZER="$ANALYZER" sh test-runner.sh "$@"
