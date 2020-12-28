SCRIPT="$0"
TESTS="$@"
CC=${CC:-gcc}
INCLUDE="$(dirname "$SCRIPT")/include"
CFLAGS=${CFLAGS:-"-Wall -Wextra -Wno-unused-parameter -fdiagnostics-color=always -g -DDEBUG -DTESTS -I$INCLUDE"}
RUNNER=
if [ -x "$(which valgrind)" ]; then
	RUNNER="valgrind --leak-check=full --show-leak-kinds=all"
fi

# Sort by Modified date
TESTS=$(stat -c '%Y %n' $TESTS | sort -t ' ' -nk1 | cut -d ' ' -f2-)

for test in $TESTS
do
	echo "${test}"
	out="$(basename ${test}.out)"
	$CC $CFLAGS $SOURCES "${test}" -o "${out}" && $RUNNER "./${out}" ; rm "${out}"
done
