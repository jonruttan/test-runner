# test-runner

A minimalist, portable unit testing framework for C.

## About The Project

`test-runner` is designed to be a lightweight and portable unit testing framework for C. It consists of a single C header file and relies on minimal standard C library functions.

## Getting Started

### Prerequisites

- A Standard C compiler
- (Recommended) A POSIX-compliant shell and utilities

### Quick Start

1. **Clone the repository:**

   ```sh
   git clone https://github.com/jonruttan/test-runner
   ```

2. **Write some tests:**

   ```sh
   mkdir tests
   cp -a test-runner/examples/minimal/*.c tests
   ```

   This creates a `tests` directory with example test files.

3. **Run the tests:**

   ```console
   $ sh test-runner/test-runner.sh tests
   tests/add-integers-test.c
   .
   OK: 1 tests (0 incomplete, 0 empty, 0 skipped), 1 assertions (0 skipped)
   ```

## Usage

### Set Up a Project

1. **Create a new project:**

   ```sh
   mkdir test-runner-project
   cd test-runner-project
   git clone https://github.com/jonruttan/test-runner
   ```

2. **Write some code to be tested:**

   ```sh
   mkdir src
   cat - > src/add-integers.c
   ```

   ```c
   int add_integers(int a, int b)
   {
         return a + b;
   }
   ```

3. **Write some tests:**

   ```sh
   mkdir tests
   cat - > tests/add-integers-test.c
   ```

   ```c
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
   ```

4. **Run the tests:**

   ```console
   $ sh test-runner/test-runner.sh tests
   tests/add-integers-test.c
   .s.ie
   WARN: 4 tests (1 incomplete, 1 empty, 1 skipped), 3 assertions (1 skipped)
   ```

   Or manually:

   ```console
   $ for test in tests/*-test.c; do echo $test; gcc -Itest-runner/include $test && ./a.out; rm a.out; done
   tests/add-integers-test.c
   .s.ie
   WARN: 4 tests (1 incomplete, 1 empty, 1 skipped), 3 assertions (1 skipped)
   ```

## Roadmap

See the [open issues](https://github.com/jonruttan/test-runner/issues) for a list of proposed features and known issues.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Contact

Jon Ruttan - [@jonruttan](https://twitter.com/jonruttan) - jonruttan@gmail.com

Project Link: [https://github.com/jonruttan/test-runner](https://github.com/jonruttan/test-runner)

## Acknowledgements

- [Test-Driven Development in C](https://eradman.com)
- [TDD in C](https://web.archive.org/web/20210325055538/https://eradman.com/tdd.html)
