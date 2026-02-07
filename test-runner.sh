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

TESTS="${@:-tests}"

OUTDIR="$(mktemp -d 2>/dev/null)" || OUTDIR="$(mktemp -d -t test-runner 2>/dev/null)" || {
	echo "ERROR: mktemp -d failed" >&2
	exit 1
}

cleanup() {
	[ -n "${OUTDIR:-}" ] && [ -d "${OUTDIR:-}" ] && rm -rf -- "$OUTDIR"
}

trap cleanup EXIT HUP INT TERM

OUTDIR="$OUTDIR" CC="$CC" CFLAGS="$CFLAGS" SOURCES="$SOURCES" WRAPPER="$WRAPPER" \
	sh test-runner-core.sh $TESTS
