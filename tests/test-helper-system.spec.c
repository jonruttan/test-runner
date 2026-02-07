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

static char *test_system_status_command_not_found(void)
{
	_it_should("not found returns 127", 127 == helper_system_status("definitely-not-a-command"));
	return NULL;
}

static char *test_system_status_signal_termination(void)
{
	/* Force the shell itself to die so WIFEXITED(rc) is false and we return 128. */
	_it_should("signal termination returns 128", 128 == helper_system_status("kill -SEGV $$"));
	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_system_status_true_false);
	_run_test(test_system_status_null_is_error);
	_run_test(test_system_status_command_not_found);
	_run_test(test_system_status_signal_termination);
	return NULL;
}
