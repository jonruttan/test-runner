# Usage

## Minimal example

```c
#include "test-runner.h"

static char *test_truth(void)
{
	_it_should("1 == 1", 1 == 1);
	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_truth);
	return NULL;
}
```

Build and run:

```sh
TESTS=your-test.c sh ./test-runner.sh
```

## Running a directory of tests

```sh
sh ./test-runner.sh tests
```

If the argument is a directory, `test-runner.sh` looks for `*.spec.c` under it.

