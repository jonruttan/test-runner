#include "test-runner.h"
#include "test-helper-system.h"

static char *test_system_status_true_false(void)
{
	_it_should("true returns 0", 0 == helper_system_status("true"));
	_it_should("false returns 1", 1 == helper_system_status("false"));
	return NULL;
}

static char *test_system_status_null_is_error(void)
{
	_it_should("NULL returns -1", -1 == helper_system_status(NULL));
	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_system_status_true_false);
	_run_test(test_system_status_null_is_error);
	return NULL;
}
