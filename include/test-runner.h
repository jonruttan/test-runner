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

/*
 * # Includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * # Constants
 */
/*
 * The current version of the Test Runner. Version numbering conforms to the
 * [Symantic Versioning] spec.
 *
 * [Symantic Versioning]: http://semver.org/
 *
 * @constant TEST_RUNNER_VERSION
 */
#define TEST_RUNNER_VERSION "1.3.0"

#define TNORM "\x1B[0m"
#define TPASS "\x1B[1;30;42m"
#define TWARN "\x1B[1;30;103m"
#define TFAIL "\x1B[1;37;41m"

#define KOK "."
#define KFAIL "x"
#define KSKIP "s"
#define KINCOMPLETE "i"
#define KEMPTY "e"

#define RUN 0
#define SKIP 1
#define INCOMPLETE 2
#define EMPTY 3

#ifndef TEST_RUNNER_OVERHEAD
static void setup(void);
static void teardown(void);
#endif /* TEST_RUNNER_OVERHEAD */

unsigned long _tests[4] = { 0, 0, 0, 0 },
	_asserts[3] = { 0, 0 },
	_current_line = 0,
	_current_column = 0;
char *_current_test = "none",
	*_current_file = "";
char *(*_dummy)(void);

#define _put_message(message) \
	do { \
		printf("%s", message); \
		if ((_current_column += strlen(message)) >= 79) { \
			_current_column =  0; \
			printf("\n"); \
		} \
	} while (0)

#define _it_should(message, test) \
	do { \
		_current_line = __LINE__; \
		_asserts[RUN]++; \
		if ( ! (test)) { \
			_put_message(KFAIL); \
			return message; \
		} \
		_put_message(KOK); \
	} while (0)

#define _mark_incomplete() \
	do { \
		_current_line = __LINE__; \
		_tests[INCOMPLETE]++; \
		_put_message(KINCOMPLETE); \
		return NULL; \
	} while (0)

#define _run_test(test) \
	do { \
		unsigned long _asserts_run = _asserts[RUN]; \
		char *_message; \
		_current_file = __FILE__; \
		_current_test = #test; \
		setup(); \
		_message = test(); \
		teardown(); \
		_tests[RUN]++; \
		if (_asserts[RUN] == _asserts_run) { \
			_tests[EMPTY]++; \
			_put_message(KEMPTY); \
		} \
		if (_message) { \
			return _message; \
		} \
	} while (0)

#define _xit_should(message, test) \
	do { \
		_current_line = __LINE__; \
		_asserts[SKIP]++; \
		_put_message(KSKIP); \
	} while (0)

#define _xmark_incomplete() {}

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
		printf(TFAIL "**FAIL**: %s(%s:%lu): it should %s" TNORM "\n",
				_current_test,
				_current_file,
				_current_line,
				result
			);

	} else {
		printf("%s: %lu tests (%lu incomplete, %lu empty, %lu skipped), %lu assertions (%lu skipped)" TNORM "\n",
				_tests[RUN] == 0 || _tests[INCOMPLETE] || _tests[EMPTY] || _tests[SKIP] || _asserts[SKIP]
					? TWARN "WARN" : TPASS "OK",
				_tests[RUN] + _tests[SKIP],
				_tests[INCOMPLETE],
				_tests[EMPTY],
				_tests[SKIP],
				_asserts[RUN] + _asserts[SKIP] + _asserts[INCOMPLETE],
				_asserts[SKIP]
			);
	}

	return 0;
}

#endif /* TEST_RUNNER_H */
