#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "lib/fbmalloc.c"
#include "lib/fbfree.c"

int main() {
    void *ptr;

    ptr = fbmalloc(5);

    printf("Address: %p\n", ptr);

    fbfree(ptr);

    return 0;
}
