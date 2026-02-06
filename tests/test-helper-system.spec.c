#include "test-runner.h"
#include "test-helper-system.h"

static char *test_system_status_true_false(void)
{
	_it_should("true returns 0", helper_system_status("true") == 0);
	_it_should("false returns 1", helper_system_status("false") == 1);
	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_system_status_true_false);
	return NULL;
}
