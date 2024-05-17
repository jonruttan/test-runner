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
#define TEST_RUNNER_VERSION "1.4.1"

#define _TNORM "\x1B[0m"
#define _TPASS "\x1B[1;30;42m"
#define _TWARN "\x1B[1;30;103m"
#define _TFAIL "\x1B[1;37;41m"

#define _KOK "."
#define _KFAIL "x"
#define _KSKIP "s"
#define _KINCOMPLETE "i"
#define _KEMPTY "e"

#define _RUN 0
#define _SKIP 1
#define _INCOMPLETE 2
#define _EMPTY 3

#ifdef TEST_RUNNER_OVERHEAD
static void _setup(void);
static void _teardown(void);
#define _SETUP() _setup();
#define _TEARDOWN() _teardown();
#else
#define _SETUP()
#define _TEARDOWN()
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
		_asserts[_RUN]++; \
		if ( ! (test)) { \
			_put_message(_KFAIL); \
			return message; \
		} \
		_put_message(_KOK); \
	} while (0)

#define _mark_incomplete() \
	do { \
		_current_line = __LINE__; \
		_tests[_INCOMPLETE]++; \
		_put_message(_KINCOMPLETE); \
		return NULL; \
	} while (0)

#define _run_test(test) \
	do { \
		unsigned long _asserts_run = _asserts[_RUN]; \
		char *_message; \
		_current_file = __FILE__; \
		_current_test = #test; \
		_SETUP() \
		_message = test(); \
		_TEARDOWN() \
		_tests[_RUN]++; \
		if (_asserts[_RUN] == _asserts_run) { \
			_tests[_EMPTY]++; \
			_put_message(_KEMPTY); \
		} \
		if (_message) { \
			return _message; \
		} \
	} while (0)

#define _xit_should(message, test) \
	do { \
		_current_line = __LINE__; \
		_asserts[_SKIP]++; \
		_put_message(_KSKIP); \
	} while (0)

#define _xmark_incomplete() {}

#define _xrun_test(test) \
	do { \
		_current_file = __FILE__; \
		_current_test = #test; \
		_tests[_SKIP]++; \
		_dummy = test; \
	} while (0)

static char *run_tests();

int main(int argc, char **argv) {
	char *result;

	srand(time(NULL));

	result = run_tests();
	printf("\n");

	if (result != 0) {
		printf(_TFAIL "**FAIL**: %s(%s:%lu): it should %s" _TNORM "\n",
				_current_test,
				_current_file,
				_current_line,
				result
			);

	} else {
		printf("%s: %lu tests (%lu incomplete, %lu empty, %lu skipped), %lu assertions (%lu skipped)" _TNORM "\n",
				_tests[_RUN] == 0 || _tests[_INCOMPLETE] || _tests[_EMPTY] || _tests[_SKIP] || _asserts[_SKIP]
					? _TWARN "WARN" : _TPASS "OK",
				_tests[_RUN] + _tests[_SKIP],
				_tests[_INCOMPLETE],
				_tests[_EMPTY],
				_tests[_SKIP],
				_asserts[_RUN] + _asserts[_SKIP],
				_asserts[_SKIP]
			);
	}

	return 0;
}

#endif /* TEST_RUNNER_H */
