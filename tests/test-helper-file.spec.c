#include "test-runner.h"
#include "test-helper-file.h"

#include <string.h>

static char *test_file_write_and_str(void)
{
	char buf[64];
	memset(buf, 0xA5, sizeof(buf));

	helper_file_buffer_ptr[TEST_HELPER_FILE_STDOUT] = buf;
	helper_file_buffer_length[TEST_HELPER_FILE_STDOUT] = sizeof(buf);
	helper_file_reset();

	_it_should("write returns size", 2 == helper_file_write(TEST_HELPER_FILE_STDOUT, "hi", 2));
	_it_should("str returns buffer", buf == helper_file_str(TEST_HELPER_FILE_STDOUT));
	_it_should("buffer contains string", 0 == strcmp(buf, "hi"));

	return NULL;
}

static char *test_file_read(void)
{
	char src[] = "abcd";
	char out[8];
	memset(out, 0, sizeof(out));

	helper_file_buffer_ptr[TEST_HELPER_FILE_STDIN] = src;
	helper_file_buffer_length[TEST_HELPER_FILE_STDIN] = 2;
	helper_file_reset();

	_it_should("read returns requested (clamped) size", 2 == helper_file_read(TEST_HELPER_FILE_STDIN, out, 4));
	_it_should("out has first 2 bytes", 'a' == out[0] && 'b' == out[1]);

	return NULL;
}

static char *test_file_no_buffer_is_noop(void)
{
	char out[8];
	memset(out, 0xA5, sizeof(out));

	helper_file_buffer_ptr[TEST_HELPER_FILE_STDIN] = NULL;
	helper_file_buffer_length[TEST_HELPER_FILE_STDIN] = TEST_HELPER_FILE_UNDEFINED;
	helper_file_reset();

	_it_should("read returns size even with NULL buffer", 4 == helper_file_read(TEST_HELPER_FILE_STDIN, out, 4));
	_it_should("write returns size even with NULL buffer", 4 == helper_file_write(TEST_HELPER_FILE_STDIN, out, 4));

	return NULL;
}

static char *run_tests(void)
{
	_run_test(test_file_write_and_str);
	_run_test(test_file_read);
	_run_test(test_file_no_buffer_is_noop);
	return NULL;
}
