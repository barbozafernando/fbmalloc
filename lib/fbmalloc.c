#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void *fbmalloc(size_t size) {
	void *p = sbrk(size);

	if (p == (void*) -1) {
		return NULL;
	} else {
		return p;
	}
}