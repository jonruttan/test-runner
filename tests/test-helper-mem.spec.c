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

static char *test_realloc_error_returns_null_and_counts(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_ERROR);

	_it_should("realloc_count starts at 0", 0 == helper_realloc_count());
	_it_should("realloc returns NULL", NULL == helper_realloc(NULL, 16));
	_it_should("realloc_count increments", 1 == helper_realloc_count());

	return NULL;
}

static char *test_hardcoded_allocators(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_HARDCODED);

	void *p = helper_malloc(16);
	_it_should("hardcoded malloc returns constant pointer", (void *)0xfacaded == p);

	void *q = helper_realloc(p, 16);
	_it_should("hardcoded realloc returns constant pointer", (void *)0xfacaded == q);
	_it_should("realloc_count increments", 1 == helper_realloc_count());

	helper_free(q);
	_it_should("free_count increments via helper_free_null", 1 == helper_free_count());

	return NULL;
}

static char *test_guaranteed_helper_malloc_guaranteed(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_GUARANTEED);

	void *p = helper_malloc_guaranteed(16);
	_it_should("helper_malloc_guaranteed returns non-null", NULL != p);
	_it_should("alloc_count increments", 1 == helper_alloc_count());

	return NULL;
}

static char *test_free_guaranteed_breaks_on_match(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_GUARANTEED);

	void *p1 = helper_malloc(8);
	void *p2 = helper_malloc(8);
	_it_should("two allocs recorded", 2 == helper_alloc_count());

	helper_free(p1);
	_it_should("free_count increments", 1 == helper_free_count());

	/*
	 * NOTE: helper_free_guaranteed() treats unknown pointers as index 0 and
	 * increments the free count. This test asserts current behavior.
	 */
	helper_free((void *)0x1234);
	_it_should("free_count increments for unknown ptr", 2 == helper_free_count());

	/* Keep p2 alive to avoid clobbering internal state between frees. */
	helper_free(p2);
	_it_should("free_count increments again", 3 == helper_free_count());

	return NULL;
}

static char *test_system_allocators_paths(void)
{
	helper_alloc_reset();
	helper_set_alloc(MEM_SYSTEM);

	void *p = helper_malloc(16);
	_it_should("system malloc returns non-null", NULL != p);

	void *q = helper_realloc(p, 32);
	_it_should("system realloc returns non-null", NULL != q);

	helper_free(q);
	_it_should("system free increments count", 1 == helper_free_count());

	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_alloc_guaranteed_counts);
	_run_test(test_alloc_error_returns_null);
	_run_test(test_realloc_count_increments);
	_run_test(test_realloc_error_returns_null_and_counts);
	_run_test(test_hardcoded_allocators);
	_run_test(test_guaranteed_helper_malloc_guaranteed);
	_run_test(test_free_guaranteed_breaks_on_match);
	_run_test(test_system_allocators_paths);
	return NULL;
}
