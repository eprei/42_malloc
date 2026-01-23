#include "malloc.h"

void *realloc(void *ptr, size_t size){
    (void) ptr;
    (void) size;
    write(1, "realloc called\n", 15);
    return NULL;
}