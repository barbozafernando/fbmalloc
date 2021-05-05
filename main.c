#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "lib/fbmalloc.c"

int main() {
    void *ptr;

    ptr = fbmalloc(5);

    printf("Address: %p", ptr);

    free(ptr);

    return 0;
}
