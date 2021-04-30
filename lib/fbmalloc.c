#include <stdio.h>
#include <malloc.h>
#include <windows.h>

void *fbmalloc(size_t size) {
	void *p = (int*) _alloca(size);

	if (p == (void*) -1) {
		return NULL;
	} else {
		return p;
	}
}