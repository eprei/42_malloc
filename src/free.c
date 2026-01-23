#include "malloc.h"

// void free(void *ptr){
//     (void) ptr;
//     write(1, "free called\n", 12);
// TODO unmap g_zones when all allocations are freed
// }