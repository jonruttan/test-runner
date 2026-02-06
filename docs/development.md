# Development

Run the repo self-tests:

```sh
sh ./test-runner.sh tests
```

## Adding tests

- Put tests under `tests/`.
- Name them `*.spec.c` so `test-runner.sh` picks them up.

## CI

GitHub Actions runs:

- `sh ./test-runner.sh tests`
- A build/run of the minimal example in `examples/minimal/`

Workflow: `.github/workflows/ci.yml`.

