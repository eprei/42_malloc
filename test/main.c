#include <stdalign.h>
#include <assert.h>

#include "../src/malloc.h"

int main() {
    write(1, "\nStarting test...\n\n", 18);

    printf("Size of size_t : %zu bytes\n", sizeof(size_t));
    printf("Size of header* : %zu bytes\n", sizeof(Header *));
    printf("Size of max_align_t : %zu bytes\n", sizeof(max_align_t));

    printf("\n-> Allocating 200 blocks of 128 bytes each (zone %d)\n", TINY);
    for (int i = 0; i < MINIMAL_ALLOCATIONS_REQUIRED * 2; i++)
    {
        // if (i % 10 == 0){
            // printf("i %d : ", i);
        // }
        void* ptr = malloc(128);
    }
    printf("\n-> Allocating 115 blocks of 129 bytes each (zone %d)\n", SMALL);
    for (int i = 0; i < 100 * 2; i++)
    {
        // if (i % 10 == 0){
            // printf("i %d : ", i);
        // }
        void* ptr = malloc(129);
    }
    printf("\n-> Allocating 3 blocks of 1025 bytes each (zone %d)\n", LARGE);
    for (int i = 0; i < 3; i++)
    {
        // printf("i %d : ", i);
        char* ptr = malloc(1025);
    }
    show_alloc_mem();
    exit(0);
    printf("\n\t -> asking for 128 bytes\n");
    void *ptr1 = malloc(128);

    printf("\n\t -> asking for 128 bytes\n");
    void *ptr2 = malloc(128);

    printf("\n\t -> asking for 128 bytes\n");
    void *ptr22 = malloc(128);

    printf("\n\t -> asking for 129 bytes\n");
    void *ptr3 = malloc(129);

    printf("\n\t -> asking for 1024 bytes\n");
    void *ptr4 = malloc(1024);

    printf("\n\t -> asking for 1025 bytes\n");
    void *ptr5 = malloc(1025);

    // printf("\n\t -> asking for 1026 bytes\n");
    // void *ptr6 = malloc(1026);

    // printf("\n\t -> asking for 1025 bytes\n");
    // void *ptr7 = malloc(1025);
    // ((char *)ptr)[2] = 'A';
    // ft_printf("%c\n",((char *)ptr)[2]);
    // printf("TINY_ZONE_MIN_SIZE = %lu bytes\n", TINY_ZONE_MIN_REQUIRED_SIZE);
    // printf("SMALL_ZONE_MIN_SIZE = %lu bytes\n", SMALL_ZONE_MIN_REQUIRED_IZE);
    // printf("Pages in TINY_ZONE_MIN_SIZE = %.6f\n", (double)  TINY_ZONE_MIN_REQUIRED_SIZE / (double) getpagesize());
    // printf("Pages in SMALL_ZONE_MIN_SIZE = %.6f\n", (double) SMALL_ZONE_MIN_REQUIRED_IZE /  (double)getpagesize());
    // printf("TINY_ZONE_ALIGNED_SIZE = %lu bytes\n", TINY_ZONE_ALIGNED_SIZE);
    // printf("SMALL_ZONE_ALIGNED_SIZE = %lu bytes\n", SMALL_ZONE_ALIGNED_SIZE);
    // printf("UNITS_IN_TINY_ZONE = %lu\n", UNITS_IN_TINY_ZONE);
    // printf("UNITS_IN_SMALL_ZONE = %lu\n", UNITS_IN_SMALL_ZONE);

    // printf("aligne of = %zu\n", _Alignof(max_align_t));
    // ptr = realloc(ptr, 192);
    // free(NULL);
    // show_alloc_mem();

    // ft_printf("Each unit/header is %u bytes long\n", sizeof(Header));
    // ft_printf("Each union header * is %u bytes long\n", sizeof(union header *));
    // ft_printf("Each size_t is %u bytes long\n", sizeof(size_t));

    // Test alignment of Header
    // _Static_assert(
    // alignof(Header) == alignof(max_align_t),
    // "Header is not maximally aligned"
// );
    // printf("Header alignment: %zu\n", _Alignof(Header));
    // printf("max_align_t alignment: %zu\n", _Alignof(max_align_t));
    return 0;
}
