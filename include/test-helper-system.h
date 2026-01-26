#ifndef TEST_HELPER_SYSTEM_H
#define TEST_HELPER_SYSTEM_H

#include "test-runner.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

/*
 * Run a shell command via system(3) and return the process exit status.
 *
 * Returns:
 * - 0..255 : program exit status (WEXITSTATUS)
 * - 127    : command could not be executed by the shell (mirrors common sh)
 * - -1     : system(3) itself failed
 */
static int helper_system_command(const char *cmd)
{
	int rc;

	rc = system(cmd);

	if (rc == -1) {
		return -1;
	}

	if (WIFEXITED(rc)) {
		return WEXITSTATUS(rc);
	}

	/* Treat signal termination / other statuses as a generic failure. */
	return 128;
}

/* Silence stdout/stderr without requiring redirection in tests. */
static int helper_system_status(const char *cmd)
{
	/*
	 * We intentionally use the shell for redirection so tests stay tiny.
	 * This is e2e-only; unit tests should avoid shelling out.
	 */
	char *buf;
	size_t n;
	int rc;

	if ( ! cmd) {
		return -1;
	}

	n = strlen(cmd) + sizeof(" >/dev/null 2>&1");
	buf = malloc(n);

	if ( ! buf) {
		return -1;
	}

	rc = snprintf(buf, n, "%s >/dev/null 2>&1", cmd);

	if (rc < 0 || (size_t)rc >= n) {
		free(buf);
		return -1;
	}

	rc = helper_system_command(buf);
	free(buf);

	return rc;
}

#endif /* TEST_HELPER_SYSTEM_H */
