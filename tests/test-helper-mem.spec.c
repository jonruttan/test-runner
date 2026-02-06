#include "test-runner.h"
#include "test-helper-mem.h"

static char *test_alloc_guaranteed_counts(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_GUARANTEED);

	_it_should("alloc_count starts at 0", 0 == helper_alloc_count());

	void *p = helper_malloc(16);
	_it_should("malloc returns non-null", NULL != p);
	_it_should("alloc_count increments", 1 == helper_alloc_count());

	helper_free(p);
	_it_should("free_count increments", 1 == helper_free_count());

	return NULL;
}

static char *test_alloc_error_returns_null(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_ERROR);
	_it_should("malloc returns NULL", NULL == helper_malloc(16));
	_it_should("alloc_count increments", 1 == helper_alloc_count());
	return NULL;
}

static char *test_realloc_count_increments(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_GUARANTEED);

	_it_should("realloc_count starts at 0", 0 == helper_realloc_count());

	void *p = helper_malloc(16);
	_it_should("malloc returns non-null", NULL != p);

	void *q = helper_realloc(p, 16);
	_it_should("realloc returns non-null", NULL != q);
	_it_should("realloc_count increments", 1 == helper_realloc_count());

	helper_free(q);
	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_alloc_guaranteed_counts);
	_run_test(test_alloc_error_returns_null);
	_run_test(test_realloc_count_increments);
	return NULL;
}
