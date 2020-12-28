#include "test-runner.h"
#include "add-integers.c"

static void setup(void)
{
}

static void teardown(void)
{
}

static char *test_add_integers(void)
{
	_it_should("add two zero integers", add_integers(1, 2) == 3);

	return NULL;
}

static char *run_tests() {
	_run_test(test_add_integers);

	return NULL;
}
