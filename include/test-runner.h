/*
 * # Test Runner in a C header
 *
 * ## test-runner.h -- Header/Implementation
 *
 * @description Test Runner in a C header
 * @author [Jon Ruttan](jonruttan@gmail.com)
 * @copyright 2020 Jon Ruttan
 * @license MIT No Attribution (MIT-0)
 *
 * Adapted from:
 *   - <https://web.archive.org/web/20140929091701/ryepdx.com/2014/09/tdd-in-c/>
 *   - <http://eradman.com/posts/tdd-in-c.html>
 *
 * ## Usage
 *
 * ### Run All Tests
 *
 * ```bash
 * for test in *-test.c; do echo $test; gcc -Wall -fdiagnostics-color=always -DTESTS -o"$test".out "$test" && ./"$test".out; rm "$test".out; done
 * ```
 */
#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TNORM "\x1B[0m"
#define TPASS "\x1B[2;30;42m"
#define TFAIL "\x1B[1;37;41m"

#define KOK "."
#define KFAIL "x"
#define KSKIP "s"

#define RUN 0
#define SKIP 1

#ifndef TEST_RUNNER_OVERHEAD
#define setup()
#define teardown()
#endif /* TEST_RUNNER_OVERHEAD */

int _tests[2] = { 0, 0 },
	_asserts[2] = { 0, 0 },
	_current_line = 0,
	_current_column = 0;
char *_current_test = "none",
	*_current_file = "";
void *_dummy;

#define _put_message(message) \
	do { \
		printf(message); \
		if((_current_column += strlen(message)) >= 79) { \
			_current_column =  0; \
			printf("\n"); \
		} \
	} while (0)

#define _it_should(message, test) \
	do { \
		_current_line = __LINE__; \
		_asserts[RUN]++; \
		if(!(test)) { \
			_put_message(KFAIL); \
			return message; \
		} \
		_put_message(KOK); \
	} while (0)

#define _run_test(test) \
	do { \
		char *message; \
		_current_file = __FILE__; \
		_current_test = #test; \
		setup(); \
		message = test(); \
		teardown(); \
		_tests[RUN]++; \
		if (message) \
			return message; \
	} while (0)

#define _xit_should(message, test) \
	do { \
		_current_line = __LINE__; \
		_asserts[SKIP]++; \
		_put_message(KSKIP); \
	} while (0)

#define _xrun_test(test) \
	do { \
		_current_file = __FILE__; \
		_current_test = #test; \
		_tests[SKIP]++; \
		_dummy = test; \
	} while (0)

static char *run_tests();

int main(int argc, char **argv) {
	char *result;

	srand(time(NULL));

	result = run_tests();
	printf("\n");

	if (result != 0) {
		printf(TFAIL "**FAIL**: %s(%s:%d): it should %s" TNORM "\n",
				_current_test,
				_current_file,
				_current_line,
				result
			);

	} else {
		printf(TPASS "OK (%d of %d tests, %d assertions, %d skipped)" TNORM "\n",
				_tests[RUN],
				_tests[RUN] + _tests[SKIP],
				_asserts[RUN],
				_asserts[SKIP]
			);
	}

	return 0;
}

#endif /* TEST_RUNNER_H */
