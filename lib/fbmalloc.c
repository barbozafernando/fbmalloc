#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>

void *fbmalloc(size_t size) {
	void *p = (int*) _alloca(size);
	int *header = p-1;

	header = (int*) _alloca(4);
	*header = size;

	if (p == (void*) -1) {
		return NULL;
	} else {
		printf("HEADER VALUE: %d\n", *header);
		return p;
	}
}