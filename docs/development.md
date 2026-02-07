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

The `container-matrix-*` targets delegate to a helper inside the container image (`test-runner-matrix`), so the container can decide which compilers are actually installed.

To see what compilers an image will use:

```sh
make container-build-alpine
docker run --rm test-runner:alpine installed-compilers.sh

make container-build-glibc
docker run --rm test-runner:glibc installed-compilers.sh
```

If you use Podman, replace `docker` with `podman` (or use `make CONTAINER=podman ...` for the Makefile targets).

To override the compiler list used by the matrix runner:

```sh
make COMPILERS='gcc clang' container-matrix-glibc
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

Note: on Debian/glibc, running clang under valgrind can produce DWARF debug-info warnings. The container's `test-runner-matrix` helper adds `-gdwarf-4` for clang when valgrind is in use to reduce this noise.

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

## Releasing

This project uses SemVer. The current version lives in `include/test-runner.h` as `TEST_RUNNER_VERSION`.

Suggested release steps:

1. Update `TEST_RUNNER_VERSION`.
2. Move relevant entries from `## Unreleased` into a new `## X.Y.Z - YYYY-MM-DD` section in `CHANGELOG.md`.
3. Run `make test`, `make example`, and `make coverage` (and optionally `make container-test-glibc`).
4. Commit the release bump.
5. Tag and push: `git tag -a vX.Y.Z -m "vX.Y.Z"` and `git push --tags`.
