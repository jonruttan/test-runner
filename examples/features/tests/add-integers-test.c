#include "test-runner.h"
#include "../src/add-integers.c"

static void setup(void) {};
static void teardown(void) {};

static char *test_add_integers(void)
{
   _it_should("add two integers", 3 == add_integers(1, 2));
   _xit_should("skip this test", 3 == add_integers(1, 1));
   return NULL;
}

static char *test_incomplete(void)
{
   _it_should("add two integers", 5 == add_integers(2, 3));
   _mark_incomplete();
   return NULL;
}

static char *test_skip(void)
{
   _it_should("skip all assertions in this test", 3 == add_integers(2, 2));
   return NULL;
}

static char *test_empty(void)
{
   /* The lack of assertions in this test will be reported. */
   return NULL;
}

static char *run_tests()
{
   _run_test(test_add_integers);
   _run_test(test_incomplete);
   _xrun_test(test_skip);
   _run_test(test_empty);
   return NULL;
}
