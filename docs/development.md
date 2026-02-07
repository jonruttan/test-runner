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

## ANALYZER Hook

`test-runner.sh` supports an optional `ANALYZER` environment variable. If set, it is executed after tests run and receives the resolved output directory as `$1`:

```sh
ANALYZER='gcovr -r . --object-directory "$1" --filter "include/" --print-summary' \
  sh ./test-runner.sh tests
```

## Commit messages

See `docs/commit_guidelines.md`.
