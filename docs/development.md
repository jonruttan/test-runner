# Development

Run the repo self-tests:

```sh
sh ./test-runner.sh
```

## Adding tests

- Put tests under `tests/`.
- Name them `*.spec.c` so `test-runner.sh` picks them up.

## CI

GitHub Actions runs:

- `sh ./test-runner.sh`
- A build/run of the minimal example in `examples/minimal/`

Workflow: `.github/workflows/ci.yml`.

## Commit messages

See `docs/commit_guidelines.md`.
