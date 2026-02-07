#!/usr/bin/env sh

# Common compile/run loop shared by test-runner.sh and other tooling (e.g. Makefile).
# This script intentionally does not create or clean up OUTDIR.

CC=${CC:-cc}
OUTDIR=${OUTDIR:-}
WRAPPER=${WRAPPER:-}

TESTS="${@:-tests}"

if [ -z "${OUTDIR:-}" ]; then
	echo "ERROR: OUTDIR is required" >&2
	exit 1
fi

if [ -d "$TESTS" ]; then
	TESTS="$TESTS/*.spec.c"
fi

# Determine correct `stat` formatting parameters
# Use GNU stat formatting
STAT="stat -c '%Y %n'"
# Try to stat this file, on error use *BSD formatting
eval $STAT "$0" 1>/dev/null 2>/dev/null || STAT="stat -f '%m %N'"

# Sort by Modified date
TESTS=$(eval $STAT $TESTS | sort -t ' ' -nk1 | cut -d ' ' -f2-)

cmd=${WRAPPER%% *}
[ -z "${WRAPPER:-}" ] || command -v "$cmd" >/dev/null 2>&1 || WRAPPER=

mkdir -p -- "$OUTDIR" || {
	echo "ERROR: could not create OUTDIR=$OUTDIR" >&2
	exit 1
}

rc=0

for test in $TESTS
do
	echo "${test}"
	out="$OUTDIR/$(basename ${test}.out)"
	$CC $CFLAGS $SOURCES "${test}" -o "${out}" && $WRAPPER "${out}" || rc=1
done

exit "$rc"
