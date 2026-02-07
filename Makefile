.DEFAULT_GOAL := help

.PHONY: help test example coverage clean \
	container-build-alpine container-test-alpine container-matrix-alpine \
	container-build-glibc container-test-glibc container-matrix-glibc container-coverage-glibc

CC ?= cc
CONTAINER ?= docker

INCLUDE_DIR := include
TEST_RUNNER_SH := ./test-runner.sh

# Default flags should match CI expectations (overrideable by env/command line).
CFLAGS ?= -Wall -Wextra -Werror -Wno-unused-parameter

TESTS_DIR := tests
TEST_GLOB := $(TESTS_DIR)/*.spec.c

COVERAGE_ANALYZER ?= gcovr
COVERAGE_CFLAGS ?= $(CFLAGS) -O0 -g --coverage
COVERAGE_FLAGS ?= --print-summary
COVERAGE_DIR ?= .coverage

# Forward common knobs into container runs so `make VAR=... container-test-*` works.
# test-runner.sh consumes: CC, CFLAGS, WRAPPER, SOURCES, ANALYZER.
# Only forward WRAPPER/SOURCES/ANALYZER* when explicitly set, to preserve defaults.
CONTAINER_ENV := -e CC="$(CC)" -e CFLAGS="$(CFLAGS)"
CONTAINER_ENV += $(if $(filter undefined,$(origin WRAPPER)),,-e WRAPPER="$(WRAPPER)")
CONTAINER_ENV += $(if $(filter undefined,$(origin SOURCES)),,-e SOURCES="$(SOURCES)")
CONTAINER_ENV += $(if $(filter undefined,$(origin ANALYZER)),,-e ANALYZER="$(ANALYZER)")
CONTAINER_ENV += $(if $(filter undefined,$(origin ANALYZER_FLAGS)),,-e ANALYZER_FLAGS="$(ANALYZER_FLAGS)")
# Coverage-related knobs for `make coverage` (always defined here, but still overrideable).
CONTAINER_ENV += -e COVERAGE_ANALYZER="$(COVERAGE_ANALYZER)" -e COVERAGE_CFLAGS="$(COVERAGE_CFLAGS)" -e COVERAGE_FLAGS="$(COVERAGE_FLAGS)"

help: ## Display this help section
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z0-9_-]+:.*?## / {printf "\033[32m%-38s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

test: ## Run self-tests (via test-runner.sh)
	sh $(TEST_RUNNER_SH) $(TESTS_DIR)

coverage: ## Run line coverage (requires gcovr)
	CFLAGS="$(COVERAGE_CFLAGS)" ANALYZER="$(COVERAGE_ANALYZER)" ANALYZER_FLAGS="$(COVERAGE_FLAGS)" sh ./test-runner-coverage.sh $(TESTS_DIR)

example: ## Build/run minimal example
	sh $(TEST_RUNNER_SH) examples/minimal/add-integers-test.c

clean: ## Remove coverage artifacts
	rm -rf -- $(COVERAGE_DIR) *.gcno *.gcda

container-build-alpine: ## Build the Alpine container image
	$(CONTAINER) build -f container/alpine/Dockerfile -t test-runner:alpine .

container-test-alpine: container-build-alpine ## Run the test suite inside the Alpine container
	$(CONTAINER) run --rm $(CONTAINER_ENV) test-runner:alpine make test

container-matrix-alpine: container-build-alpine ## Run the test suite inside Alpine using gcc/clang/tcc
	$(CONTAINER) run --rm test-runner:alpine sh -lc 'for cc in gcc clang tcc; do command -v $$cc >/dev/null 2>&1 || { echo "skip: $$cc not installed"; continue; }; echo "CC=$$cc"; CC=$$cc make test; done'

container-build-glibc: ## Build the Debian (glibc) container image
	$(CONTAINER) build -f container/debian/Dockerfile -t test-runner:glibc .

container-test-glibc: container-build-glibc ## Run the test suite inside the Debian (glibc) container
	$(CONTAINER) run --rm $(CONTAINER_ENV) test-runner:glibc make test

container-matrix-glibc: container-build-glibc ## Run the test suite inside Debian (glibc) using gcc/clang/tcc
	$(CONTAINER) run --rm test-runner:glibc sh -lc 'for cc in gcc clang tcc; do echo "CC=$$cc"; CC=$$cc make test; done'

container-coverage-glibc: container-build-glibc ## Run coverage inside Debian (glibc) container
	$(CONTAINER) run --rm $(CONTAINER_ENV) test-runner:glibc make coverage
