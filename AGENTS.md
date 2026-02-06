# AGENTS.md

Project-specific instructions for AI agents working in this repository.

## What This Repo Is

- A small, vendorable test runner implemented primarily in `include/test-runner.h`.
- A tiny set of optional helper headers in `include/test-helper-*.h`.

## Local Commands

- Run self-tests: `sh ./test-runner.sh`
- Build/run the minimal example:
  - `sh ./test-runner.sh examples/minimal/add-integers-test.c`

## Engineering Rules

- Keep `include/test-runner.h` lightweight and dependency-minimal.
- Headers must be self-contained: if a header uses a symbol, it must directly include the header that declares it.
- Prefer POSIX `sh` for scripts.
- If behavior changes, add/adjust tests under `tests/` (`*.spec.c`).

## Change Hygiene

- Keep diffs focused on the requested work.
- If unrelated modifications are present, do not revert them automatically; call them out.

## Commits

If asked to create commits, follow `docs/commit_guidelines.md`.
