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
CC=${CC:-gcc}
INCLUDE="$(dirname "$SCRIPT")/include"
CFLAGS=${CFLAGS:-"-Wall -Wextra -Wno-unused-parameter"}
CFLAGS="${CFLAGS} -g -fdiagnostics-color=always -DDEBUG -DTESTS -I$INCLUDE"
if [ -x "$(which valgrind)" ]; then
	VALGRIND="valgrind --quiet --show-error-list=yes --leak-check=full --show-leak-kinds=all"
fi
RUNNER=${RUNNER:-${VALGRIND}}

TESTS="$@"

# If $TESTS is a directory search it for tests
if [ -d "$TESTS" ]; then
	TESTS="$TESTS/*-test.c"
fi

# Determine correct `stat` formatting parameters
# Use GNU stat formatting
STAT="stat -c '%Y %n'"
# Try to stat this file, on error use *BSD formatting
eval $STAT "$0" 1>/dev/null 2>/dev/null || STAT="stat -f '%m %N'"

# Sort by Modified date
TESTS=$(eval $STAT $TESTS | sort -t ' ' -nk1 | cut -d ' ' -f2-)

for test in $TESTS
do
	echo "${test}"
	out="$(basename ${test}.out)"
	$CC $CFLAGS $SOURCES "${test}" -o "${out}" && $RUNNER "./${out}"
	[ -x ] && rm "${out}"
done
