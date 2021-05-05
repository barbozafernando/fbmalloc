#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>

struct memory_data {
	int *address;
	size_t bytes;
};

struct memory_data memdata;

void *fbmalloc(size_t size) {
	void *p = (int*) _alloca(size);

	if (p == (void*) -1) {
		return NULL;
	} else {
		memdata.address = p;
		memdata.bytes = size;
		return p;
	}
}

void *fbfree(*ptr) {
	
}