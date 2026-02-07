# Changelog

This project intends to follow semantic versioning.

## Unreleased

- (none)

## 1.7.0 - 2026-02-07

- Add a Makefile and update CI to use it (`make test`, `make example`, `make coverage`).
- Add an `ANALYZER` hook to `test-runner.sh` and a `test-runner-coverage.sh` wrapper (gcovr).
- Add container-based testing targets (Alpine and Debian/glibc), including glibc-container coverage.
- Fix memory helper accounting for freeing unknown pointers.
- Improve self-test coverage for the runner and helpers.
- Expand development/usage documentation (targets, containers, environment variables).

## 1.6.1 - 2026-02-06

- Fix `helper_realloc_count()` to report reallocs correctly.
- Fix missing direct includes in helper headers.
- Add GitHub Actions CI and self-tests.
- Add commit message guidelines and project docs.
- Ignore `.env`.
