#include "test-runner.h"
#include "add-integers.c"

static char *test_add_integers(void)
{
	_it_should("add two zero integers", 3 == add_integers(1, 2));

	return NULL;
}

static char *run_tests() {
	_run_test(test_add_integers);

	return NULL;
}
