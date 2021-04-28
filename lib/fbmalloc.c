#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void *fbmalloc(size_t size) {
	void *p = sbrk(0);
	void *request = sbrk(size);

	if (request == (void*) -1) {
		return NULL;
	} else {
		assert(p == request);
		return p;
	}
}