#define TEST_RUNNER_OVERHEAD
#include "test-runner.h"

static int setup_calls = 0;
static int teardown_calls = 0;

static void _setup(void)
{
	setup_calls++;
}

static void _teardown(void)
{
	teardown_calls++;
}

static char *test_overhead_hooks_run(void)
{
	_it_should("sanity", 1 == 1);
	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_overhead_hooks_run);

	if (setup_calls != 1) {
		return "setup called exactly once";
	}

	if (teardown_calls != 1) {
		return "teardown called exactly once";
	}

	return NULL;
}

