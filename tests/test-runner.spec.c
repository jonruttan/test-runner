#include "test-runner.h"

static char *test_it_should_counts_asserts(void)
{
	_it_should("1 == 1", 1 == 1);
	_it_should("2 == 2", 2 == 2);
	return NULL;
}

static char *test_mark_incomplete_counts(void)
{
	_it_should("assert before incomplete", 1 == 1);
	_mark_incomplete();
	return NULL;
}

static char *test_empty_is_reported(void)
{
	/* Intentionally no assertions. */
	return NULL;
}

static char *test_xit_should_counts_skipped_assert(void)
{
	_xit_should("skipped assert increments skipped counter", 0);
	return NULL;
}

static int _side_effect_ran = 0;

static char *test_side_effect(void)
{
	_side_effect_ran = 1;
	return NULL;
}

static char *run_tests(void)
{
	#define EXPECT(msg, cond) do { if (!(cond)) return (char *)(msg); } while (0)

	EXPECT("initial _tests[_RUN] == 0", _tests[_RUN] == 0);
	EXPECT("initial _tests[_SKIP] == 0", _tests[_SKIP] == 0);
	EXPECT("initial _tests[_INCOMPLETE] == 0", _tests[_INCOMPLETE] == 0);
	EXPECT("initial _tests[_EMPTY] == 0", _tests[_EMPTY] == 0);
	EXPECT("initial _asserts[_RUN] == 0", _asserts[_RUN] == 0);
	EXPECT("initial _asserts[_SKIP] == 0", _asserts[_SKIP] == 0);

	_run_test(test_it_should_counts_asserts);
	EXPECT("after asserts: _tests[_RUN] == 1", _tests[_RUN] == 1);
	EXPECT("after asserts: _asserts[_RUN] == 2", _asserts[_RUN] == 2);
	EXPECT("after asserts: _tests[_EMPTY] == 0", _tests[_EMPTY] == 0);

	_run_test(test_mark_incomplete_counts);
	EXPECT("after incomplete: _tests[_RUN] == 2", _tests[_RUN] == 2);
	EXPECT("after incomplete: _tests[_INCOMPLETE] == 1", _tests[_INCOMPLETE] == 1);
	EXPECT("after incomplete: _asserts[_RUN] == 3", _asserts[_RUN] == 3);

	_xrun_test(test_side_effect);
	EXPECT("_xrun_test increments skip tests", _tests[_SKIP] == 1);
	EXPECT("_xrun_test does not execute test", _side_effect_ran == 0);

	_run_test(test_empty_is_reported);
	EXPECT("empty test increments empty count", _tests[_EMPTY] == 1);
	EXPECT("empty test still increments run count", _tests[_RUN] == 3);

	_run_test(test_xit_should_counts_skipped_assert);
	EXPECT("xit increments skipped asserts", _asserts[_SKIP] == 1);
	EXPECT("xit test increments run count", _tests[_RUN] == 4);

	/*
	 * These tests intentionally exercised skip/incomplete/empty accounting.
	 * Reset the summary-impacting counters so this self-test suite reports OK
	 * (while still covering the code paths above).
	 */
	_tests[_INCOMPLETE] = 0;
	_tests[_EMPTY] = 0;
	_tests[_SKIP] = 0;
	_asserts[_SKIP] = 0;

	return NULL;
}
