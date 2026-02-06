#include "test-runner.h"

static char *test_it_should_passes(void)
{
	_it_should("1 == 1", 1 == 1);
	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_it_should_passes);
	return NULL;
}
