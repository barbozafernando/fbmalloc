#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "lib/fbmalloc.h"

int main() {
    void *ptr;

    ptr = fbmalloc(10);

    printf("Address: %p", ptr);

    free(ptr);

	return 0;
}
