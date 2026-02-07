# Development

## Quick commands

Run the repo self-tests:

```sh
make test
```

Build and run the minimal example:

```sh
make example
```

Run line coverage (requires `gcovr`):

```sh
make coverage
```

## Container testing

These targets run the test suite in a container for a minimal environment and compiler variation.
The container engine defaults to `docker`, but you can use `podman` by setting `CONTAINER=podman`.

```sh
make container-test-alpine
make container-matrix-alpine

make container-test-glibc
make container-matrix-glibc
make container-coverage-glibc
```

To target a specific compiler inside a container, set `CC`:

```sh
make CC=clang container-test-glibc
make CC=gcc container-test-alpine
```

Other useful knobs are also forwarded into the container run (matching `test-runner.sh`):

```sh
make CFLAGS='-std=c11 -Wall -Wextra -Werror' container-test-glibc
make WRAPPER='valgrind --quiet --leak-check=full --show-leak-kinds=all' container-test-glibc
make SOURCES='src/foo.c src/bar.c' container-test-alpine
```

## Adding tests

- Put tests under `tests/`.
- Name them `*.spec.c` so `test-runner.sh` picks them up.

## CI

GitHub Actions runs:

- `make test`
- `make example`
- `make coverage` (Ubuntu-only)

Workflow: `.github/workflows/ci.yml`.

## Coverage Notes

Coverage is driven by `test-runner-coverage.sh`, which configures coverage-friendly `CFLAGS` and uses the `ANALYZER` hook in `test-runner.sh` to run `gcovr` after the tests finish but before cleanup.

To install `gcovr` locally:

```sh
python3 -m pip install --user gcovr
```

## Environment variables

### Makefile variables

- `CC`: compiler to use (default: `cc`)
- `CFLAGS`: compiler flags used by `make test` (the test runner will append its own flags)
- `CONTAINER`: container engine (default: `docker`, set to `podman` to use Podman)
- `COVERAGE_ANALYZER`: coverage analyzer executable (default: `gcovr`)
- `COVERAGE_CFLAGS`: flags used by `make coverage` (defaults to `$(CFLAGS) -O0 -g --coverage`)
- `COVERAGE_FLAGS`: flags passed to the analyzer via `ANALYZER_FLAGS` (default: `--print-summary`)

### test-runner.sh variables

`test-runner.sh` uses these variables:

- `CC`: compiler executable (default: `cc`)
- `CFLAGS`: extra compiler flags (the script also adds `-DDEBUG -DTESTS -Iinclude` and `-g`)
- `SOURCES`: extra `.c` sources to compile/link into each test binary
- `WRAPPER`: command used to run each compiled test binary
  - default: if `valgrind` is installed, the script uses it automatically
  - set `WRAPPER=` to run the test binary directly (disable valgrind)
- `ANALYZER`: optional shell snippet run after tests and before cleanup
  - it receives the resolved output directory as `$1`
  - it is executed as `sh -c "$ANALYZER" sh "$OUTDIR"`

Examples:

```sh
CC=clang sh ./test-runner.sh tests
WRAPPER= sh ./test-runner.sh tests
SOURCES='src/foo.c src/bar.c' sh ./test-runner.sh tests
ANALYZER='gcovr -r . --object-directory "$1" --filter "include/" --print-summary' sh ./test-runner.sh tests
```

### test-runner-coverage.sh variables

`test-runner-coverage.sh` is a convenience wrapper around `test-runner.sh`:

- `ANALYZER`: analyzer executable (default: `gcovr`)
- `ANALYZER_FLAGS`: analyzer flags (default: `--print-summary`)
- `CFLAGS`: if unset, it defaults to coverage-friendly flags (including `--coverage`)

Example:

```sh
ANALYZER=gcovr ANALYZER_FLAGS='--print-summary' sh ./test-runner-coverage.sh tests
```

## Commit messages

See `docs/commit_guidelines.md`.
