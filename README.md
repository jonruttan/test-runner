# test-runner

A minimalist, portable unit testing framework for C.

## About The Project

test-runner is a minimalist, portable unit testing framework for C. The implementation is a single C header file, and its core depends on only a few standard C library functions.

## Getting Started

### Prerequisites

- A Standard C compiler
- (Optional) A [POSIX](https://en.wikipedia.org/wiki/POSIX) compliant shell and utilities

### QuickStart

1. Clone the repo

   ```sh
   git clone https://github.com/jonruttan/test-runner
   ```
2. Write some tests
   ```sh
   mkdir tests
   cp -a test-runner/examples/minimal/*.c tests
   tree tests # list contents of directories in a tree-like format
   ```
   *output*
   
   ```
   tests
   ├── add-integers.c
   └── add-integers-test.c
   
   0 directories, 2 files
   ```
3. Run the tests
   ```sh
   sh test-runner/test-runner.sh tests
   ```
   *output*
   
   ```
   tests/add-integers-test.c
   .
   OK (1 of 1 tests, 1 assertions, 0 skipped)
   ```

## Usage

Set up a project

```sh
mkdir test-runner-project
cd test-runner-project
git clone https://github.com/jonruttan/test-runner
```

Write some code to be tested

```sh
mkdir src
cat - > src/add-integers.c # Paste the code from below, finish with Ctrl-d
```

```c
int add_integers(int a, int b)
{
	return a + b;
}

```

Write some tests

```sh
mkdir tests
cat - > tests/add-integers-test.c # Paste the code from below, finish with Ctrl-d
```

```c
#include "test-runner.h"
#include "../src/add-integers.c"

static char *test_add_integers(void)
{
	_it_should("add two integers", add_integers(1, 2) == 3);
	_xit_should("skip this test", add_integers(1, 1) == 3);
    
	return NULL;
}

static char *test_skip(void)
{
	_it_should("skip this test as well", exit(-1));

	return NULL;
}

static char *run_tests() {
	_run_test(test_add_integers);
	_xrun_test(test_skip);

	return NULL;
}

```

Run the tests


```sh
# Use the test runner shell script
sh test-runner/test-runner.sh tests

# Or run the tests by hand
for test in tests/*-test.c; do echo $test; gcc -Wall -fdiagnostics-color=always -DTESTS -Itest-runner/include $test && ./a.out; rm a.out; done
```

*output*

```
tests/add-integers-test.c
.s
OK (1 of 2 tests, 1 assertions, 1 skipped)
```

## Roadmap

See the [open issues](https://github.com/jonruttan/test-runner/issues) for a list of proposed features (and known issues).


## Contributing

Contributions are welcome.

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request


## License

Distributed under the MIT License. See `LICENSE` for more information.


## Contact

Jon Ruttan - [@jonruttan](https://twitter.com/jonruttan) - jonruttan@gmail.com

Project Link: <https://github.com/jonruttan/test-runner>


## Acknowledgements

* [Test-Driven Development in C](http://eradman.com/posts/tdd-in-c.html)
* [TDD in C](https://web.archive.org/web/20141007161142/ryepdx.com/2014/09/tdd-in-c/)
