#include "test-runner.h"
#include "test-helper-mem.h"

static char *test_alloc_guaranteed_counts(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_GUARANTEED);

	_it_should("alloc_count starts at 0", helper_alloc_count() == 0);

	void *p = helper_malloc(16);
	_it_should("malloc returns non-null", p != NULL);
	_it_should("alloc_count increments", helper_alloc_count() == 1);

	helper_free(p);
	_it_should("free_count increments", helper_free_count() == 1);

	return NULL;
}

static char *test_alloc_error_returns_null(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_ERROR);
	_it_should("malloc returns NULL", helper_malloc(16) == NULL);
	_it_should("alloc_count increments", helper_alloc_count() == 1);
	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_alloc_guaranteed_counts);
	_run_test(test_alloc_error_returns_null);
	return NULL;
}
