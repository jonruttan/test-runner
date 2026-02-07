#!/usr/bin/env sh

#
# # Test Runner in a C header
#
# ## test-runner.sh -- Shell Script - Runner
#
# @description Test Runner in a C header
# @author [Jon Ruttan](jonruttan@gmail.com)
# @copyright 2020 Jon Ruttan
# @license MIT No Attribution (MIT-0)
#
# ## Usage
#
# ### Run All Tests
#
# ```bash
# sh test-runner.sh
# ```
#
SCRIPT="$0"
CC=${CC:-cc}
INCLUDE="$(dirname "$SCRIPT")/include"
CFLAGS=${CFLAGS:-"-Wall -Wextra -Wno-unused-parameter"}
CFLAGS="${CFLAGS} -g -fdiagnostics-color=always -DDEBUG -DTESTS -I$INCLUDE"
if [ -x "$(which valgrind)" ]; then
	VALGRIND="valgrind --quiet --leak-check=full --show-leak-kinds=all"
fi
WRAPPER=${WRAPPER:-${VALGRIND}}
ANALYZER=${ANALYZER:-}

TESTS="${@:-tests}"

# If $TESTS is a directory search it for tests
if [ -d "$TESTS" ]; then
	TESTS="$TESTS/*.spec.c"
fi

OUTDIR="$(mktemp -d 2>/dev/null)" || OUTDIR="$(mktemp -d -t test-runner 2>/dev/null)" || {
	echo "ERROR: mktemp -d failed" >&2
	exit 1
}

cleanup() {
	[ -n "${OUTDIR:-}" ] && [ -d "${OUTDIR:-}" ] && rm -rf -- "$OUTDIR"
}

trap cleanup EXIT HUP INT TERM

# Determine correct `stat` formatting parameters
# Use GNU stat formatting
STAT="stat -c '%Y %n'"
# Try to stat this file, on error use *BSD formatting
eval $STAT "$0" 1>/dev/null 2>/dev/null || STAT="stat -f '%m %N'"

# Sort by Modified date
TESTS=$(eval $STAT $TESTS | sort -t ' ' -nk1 | cut -d ' ' -f2-)

cmd=${WRAPPER%% *}
[ -z "${WRAPPER:-}" ] || command -v "$cmd" >/dev/null 2>&1 || WRAPPER=

rc=0

for test in $TESTS
do
	echo "${test}"
	out="$OUTDIR/$(basename ${test}.out)"
	$CC $CFLAGS $SOURCES "${test}" -o "${out}" && $WRAPPER "${out}" || rc=1
	# [ -x "${out}" ] && rm "${out}"
	# [ -d "${out}.dSYM" ] && rm -Rf "${out}.dSYM"
done

# ANALYZER is a shell snippet that receives OUTDIR as $1 and runs before cleanup.
# Example: ANALYZER='gcovr -r . --object-directory "$1" --filter "include/" --print-summary'
[ -z "${ANALYZER:-}" ] || sh -c "$ANALYZER" sh "$OUTDIR"

exit "$rc"
