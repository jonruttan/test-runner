.DEFAULT_GOAL := help

.PHONY: help test example coverage clean

CC ?= cc

INCLUDE_DIR := include
TEST_RUNNER_SH := ./test-runner.sh

# Default flags should match CI expectations (overrideable by env/command line).
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -Wno-unused-parameter

TESTS_DIR := tests
TEST_GLOB := $(TESTS_DIR)/*.spec.c

COVERAGE_DIR := .coverage
COVERAGE_CFLAGS ?= -std=c11 -Wall -Wextra -Wno-unused-parameter -O0 -g --coverage
COVERAGE_FLAGS ?= --print-summary

help: ## Display this help section
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z0-9_-]+:.*?## / {printf "\033[32m%-38s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

test: ## Run self-tests (via test-runner.sh)
	sh $(TEST_RUNNER_SH) $(TESTS_DIR)

example: ## Build/run minimal example
	sh $(TEST_RUNNER_SH) examples/minimal/add-integers-test.c

coverage: ## Run line coverage (requires gcovr)
	OUTDIR=$(COVERAGE_DIR) CC=$(CC) CFLAGS="$(COVERAGE_CFLAGS)" GCOVR=gcovr \
		sh ./test-runner-coverage.sh $(TESTS_DIR)

clean: ## Remove coverage artifacts
	rm -rf $(COVERAGE_DIR)
