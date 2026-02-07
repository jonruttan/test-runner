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

## test-runner.sh environment variables

`test-runner.sh` can be configured via environment variables:

- `CC`: compiler to use (default: `cc`)
- `CFLAGS`: extra compiler flags (the script also adds `-DDEBUG -DTESTS -Iinclude` and `-g`)
- `SOURCES`: extra `.c` sources to compile/link into each test binary
- `WRAPPER`: command used to run each compiled test binary
  - default: if `valgrind` is present, the script uses it automatically
  - set `WRAPPER=` to run the test binary directly (disable valgrind)
- `ANALYZER`: optional shell snippet run after tests and before cleanup; receives the output directory as `$1`

Examples:

```sh
CC=clang sh ./test-runner.sh tests
WRAPPER= sh ./test-runner.sh tests
SOURCES='src/foo.c src/bar.c' sh ./test-runner.sh tests
ANALYZER='gcovr -r . --object-directory "$1" --filter "include/" --print-summary' sh ./test-runner.sh tests
```
