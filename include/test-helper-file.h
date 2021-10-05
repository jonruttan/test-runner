#ifndef TEST_HELPER_FILE_H
#define TEST_HELPER_FILE_H

#define TEST_HELPER_FILE_DESCRIPTORS 8

#define TEST_HELPER_FILE_READ 0
#define TEST_HELPER_FILE_WRITE 1

#define TEST_HELPER_FILE_STDIN 0
#define TEST_HELPER_FILE_STDOUT 1
#define TEST_HELPER_FILE_STDERR 2
#define TEST_HELPER_FILE_FILE1 3
#define TEST_HELPER_FILE_FILE2 4
#define TEST_HELPER_FILE_FILE3 5
#define TEST_HELPER_FILE_FILE4 6
#define TEST_HELPER_FILE_FILE5 7

char *helper_file_buffer_ptr[TEST_HELPER_FILE_DESCRIPTORS] = {
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	},
	*file_buffer_ptr[TEST_HELPER_FILE_DESCRIPTORS][2];

void helper_file_reset(void)
{
	int fd;

	for (fd=TEST_HELPER_FILE_DESCRIPTORS; fd--;) {
		file_buffer_ptr[fd][TEST_HELPER_FILE_READ] = helper_file_buffer_ptr[fd];
		file_buffer_ptr[fd][TEST_HELPER_FILE_WRITE] = helper_file_buffer_ptr[fd];
	}
}

char *helper_file_str(int fd)
{
	if (helper_file_buffer_ptr[fd]) {
		*file_buffer_ptr[fd][TEST_HELPER_FILE_WRITE] = 0;
	}

	return helper_file_buffer_ptr[fd];
}

ssize_t helper_file_read(int fd, void *p_buf, size_t size)
{
	if (helper_file_buffer_ptr[fd]) {
		memcpy(p_buf, file_buffer_ptr[fd][TEST_HELPER_FILE_READ], size);
		file_buffer_ptr[fd][TEST_HELPER_FILE_READ] += size;
	}

	return size;
}

ssize_t helper_file_write(int fd, const void *p_buf, size_t size)
{
	if (helper_file_buffer_ptr[fd]) {
		memcpy(file_buffer_ptr[fd][TEST_HELPER_FILE_WRITE], p_buf, size);
		file_buffer_ptr[fd][TEST_HELPER_FILE_WRITE] += size;
	}

	return size;
}

#endif /* TEST_HELPER_FILE_H */
