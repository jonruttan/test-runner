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
RUNNER=
CFLAGS=${CFLAGS:-"-Wall -Wextra -Wno-unused-parameter"}
CFLAGS="${CFLAGS} -g -fdiagnostics-color=always -DDEBUG -DTESTS -I$INCLUDE"
if [ -x "$(which valgrind)" ]; then
	RUNNER="valgrind --leak-check=full --show-leak-kinds=all"
fi

TESTS="$@"

# If $TESTS is a directory search it for tests
if [ -d "$TESTS" ]; then
	TESTS="$TESTS/*-test.c"
fi

# Sort by Modified date
TESTS=$(stat -c '%Y %n' $TESTS | sort -t ' ' -nk1 | cut -d ' ' -f2-)

for test in $TESTS
do
	echo "${test}"
	out="$(basename ${test}.out)"
	$CC $CFLAGS $SOURCES "${test}" -o "${out}" && $RUNNER "./${out}" ; rm "${out}"
done
