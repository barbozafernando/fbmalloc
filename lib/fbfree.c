#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>

int fbfree(void *ptr) {
    int *headers = ptr-1;

	printf("FREE: %p\n", ptr);
    printf("HEADER2222: %p\n", ptr-1);
    printf("HEADER VAL: %p\n", *headers);

    return 0;
}