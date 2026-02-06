#ifndef TEST_HELPER_MEM_H
#define TEST_HELPER_MEM_H

#include "test-runner.h"
#include <stdlib.h> /* calloc, realloc, free */
#include <string.h> /* memset, memcpy */

#define FREE_CHAR 0xFC

#define BUFFERS 1024
#define ALLOC_SIZE 4096

typedef void *(*helper_alloc_fn)(size_t, size_t);
typedef void *(*helper_realloc_fn)(void *, size_t);
typedef void (*helper_free_fn)(void *);

unsigned char _alloc_buffer[BUFFERS][ALLOC_SIZE];
void *_alloc_ptrs[BUFFERS];
int _buffer_index = -1,
	_buffer_alloc_count = 0,
	_buffer_realloc_count = 0,
	_buffer_free_count = 0;
helper_alloc_fn helper_alloc = calloc;
helper_realloc_fn helper_realloc = realloc;
helper_free_fn helper_free = free;

enum { MEM_SYSTEM, MEM_GUARANTEED, MEM_ERROR, MEM_HARDCODED };

void *helper_alloc_guaranteed(size_t nmemb, size_t size)
{
	_buffer_index++;
	_buffer_alloc_count++;
	memset(_alloc_buffer[_buffer_index], 0, nmemb * size);
	memset(_alloc_buffer[_buffer_index] + (nmemb * size), FREE_CHAR, ALLOC_SIZE - (nmemb * size));
	return (_alloc_ptrs[_buffer_index] = _alloc_buffer[_buffer_index]);
}
void *helper_malloc_guaranteed(size_t size)
{
	return helper_alloc_guaranteed(1, size);
}

void *helper_realloc_guaranteed(void *ptr, size_t size)
{
	int src = _buffer_index;
	int dst = ++_buffer_index;
	_buffer_realloc_count++;
	memcpy(_alloc_buffer[dst], _alloc_buffer[src], size);
	memset(_alloc_buffer[dst] + size, FREE_CHAR, ALLOC_SIZE - size);
	return (_alloc_ptrs[_buffer_index] = _alloc_buffer[_buffer_index]);
}

void helper_free_guaranteed(void *ptr)
{
	int i;

	for (i=_buffer_index; i; i--) {
		if (_alloc_ptrs[i] == ptr) {
			break;
		}
	}

	if (i >= 0) {
		memset(_alloc_buffer[i], FREE_CHAR, ALLOC_SIZE);
		_buffer_free_count++;
	}
}

void *helper_alloc_null(size_t nmemb, size_t size)
{
	_buffer_alloc_count++;
	return NULL;
}

void *helper_realloc_null(void *ptr, size_t size)
{
	_buffer_realloc_count++;
	return NULL;
}

void helper_free_null(void *ptr)
{
	_buffer_free_count++;
}

void *helper_alloc_hardcoded(size_t nmemb, size_t size)
{
	_buffer_alloc_count++;
	return (void *)0xfacaded;
}

void *helper_realloc_hardcoded(void *ptr, size_t size)
{
	_buffer_realloc_count++;
	return (void *)0xfacaded;
}

void *helper_alloc_system(size_t nmemb, size_t size)
{
	_buffer_alloc_count++;
	return calloc(nmemb, size);
}

void *helper_realloc_system(void *ptr, size_t size)
{
	_buffer_realloc_count++;
	return realloc(ptr, size);
}

void helper_free_system(void *ptr)
{
	_buffer_free_count++;
	free(ptr);
}

void helper_set_alloc(int type)
{
	switch(type) {
	case MEM_GUARANTEED:
		helper_alloc = helper_alloc_guaranteed;
		helper_realloc = helper_realloc_guaranteed;
		helper_free = helper_free_guaranteed;
		break;

	case MEM_ERROR:
		helper_alloc = helper_alloc_null;
		helper_realloc = helper_realloc_null;
		helper_free = helper_free_null;
		break;

	case MEM_HARDCODED:
		helper_alloc = helper_alloc_hardcoded;
		helper_realloc = helper_realloc_hardcoded;
		helper_free = helper_free_null;
		break;

	case MEM_SYSTEM:
	default:
		helper_alloc = helper_alloc_system;
		helper_realloc = helper_realloc_system;
		helper_free = helper_free_system;
		break;
	}
}

void helper_alloc_reset(void)
{
	_buffer_index = -1;
	_buffer_alloc_count = 0;
	_buffer_realloc_count = 0;
	_buffer_free_count = 0;
}

int helper_alloc_count(void)
{
	return _buffer_alloc_count;
}

int helper_realloc_count(void)
{
	return _buffer_alloc_count;
}

int helper_free_count(void)
{
	return _buffer_free_count;
}

void *helper_malloc(size_t size)
{
	return helper_alloc(1, size);
}

#endif /* TEST_HELPER_MEM_H */
