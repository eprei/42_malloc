#include <time.h>

#include "../src/malloc.h"

# define ALLOCATED_AMOUNT 5

static char *zone_names[] = {"TINY", "SMALL", "LARGE"};

static void test_double_free(void){
    printf("\n\tTesting double free\n\n");

    void *ptr = malloc(200);
    printf("200 bytes has been allocated at 0x%lX\n\n", ptr);
    show_alloc_mem();
    printf("\nFreeing 0x%lX first time\n", ptr);
    free(ptr);
    printf("Freeing 0x%lX second time\n", ptr);
    free(ptr);
}

static void test_invalid_pointers(void){
    printf("\n\tTesting invalid pointers\n\n");

    printf("Freeing NULL pointer\n");
    void *ptr = NULL;
    free(ptr);
    printf("\n");

    printf("Freeing invalid pointer 0xDEADBEEF\n");
    free((void *)0xDEADBEEF);
    printf("\n");

    printf("Freeing pointer not from malloc\n");
    int x = 42;
    free(&x);
    printf("\n");

    printf("Freeing pointer offset from malloced block\n");
    char *ptr2 = malloc(100);
    printf("ptr2: 0x%lX\n", (unsigned long)ptr2);
    free(ptr2 + 50);
    free(ptr2);
}

static void test_zone(int zone_index){
    int size;
    char *zone_name = zone_names[zone_index];

    switch (zone_index){
        case TINY:
            size = TINY_MAX_BYTE_SIZE - 1;
            break;
        case SMALL:
            size = SMALL_MAX_BYTE_SIZE - 1;
            break;
        case LARGE:
            size = SMALL_MAX_BYTE_SIZE + 1;
            break;
    }

    printf("\n\tTesting %s zone\n\n", zone_name);

    printf("Allocating %d block(s) of %d bytes each (zone %s)\n\n", ALLOCATED_AMOUNT, size, zone_name);
    char *ptr[ALLOCATED_AMOUNT];
    for (int i = 0; i < ALLOCATED_AMOUNT; i++){
        ptr[i] = malloc(size);
        // int r = rand() % 99;
        // ptr2[r] = 'B';
        // printf("%c\n", ptr2[r]);
    }
    show_alloc_mem();

    for (int i = 0; i < ALLOCATED_AMOUNT; i++){
        free(ptr[i]);
    }
}

static void test_realloc(void){
    printf("\n\tTesting realloc: when there is space to expand\n\n");
    printf("Allocating 50 bytes two times\n\n");
    void *ptr2 = malloc(50);
    void *ptr3 = malloc(50);
    show_alloc_mem();
    printf("\nFreeing 0x%lX to create space\n\n", ptr2);
    free(ptr2);
    show_alloc_mem();
    printf("\nRealloc 0x%lX to 100 bytes\n\n", ptr3);
    ptr3 = realloc(ptr3, 100);
    show_alloc_mem();
    free(ptr3);

    printf("\n\tTesting realloc: when no space to expand\n\n");
    printf("Allocating 50 bytes\n\n");
    void *ptr = malloc(50);
    show_alloc_mem();
    printf("\nRealloc 0x%lX to 100 bytes\n\n", ptr);
    ptr = realloc(ptr, 100);
    show_alloc_mem();
    free(ptr);

    void *invalid_ptr = (void *)0x12345678;
    printf("\n\n\tTesting realloc: invalid pointer %p\n\n", invalid_ptr);
    realloc(invalid_ptr, 150);

}

static void test_mixed_allocations(void){
    const int size_tiny = TINY_MAX_BYTE_SIZE - 1;
    const int size_small = SMALL_MAX_BYTE_SIZE - 1;
    const int size_large = SMALL_MAX_BYTE_SIZE + 1;
    char *ptr_tiny[ALLOCATED_AMOUNT];
    char *ptr_small[ALLOCATED_AMOUNT];
    char *ptr_large[ALLOCATED_AMOUNT];

    printf("\n\tTesting mixed allocations\n\n");

    printf("Allocating %d block(s) for each zone\n\n", ALLOCATED_AMOUNT);
    for (int i = 0; i < ALLOCATED_AMOUNT; i++){
        ptr_tiny[i] = malloc(size_tiny);
        ptr_small[i] = malloc(size_small);
        ptr_large[i] = malloc(size_large);
    }

    show_alloc_mem();

    for (int i = 0; i < ALLOCATED_AMOUNT; i++){
        free(ptr_tiny[i]);
        free(ptr_small[i]);
        free(ptr_large[i]);
    }
}

static void test_defragmentation(void){
    printf("\n\tTesting defragmentation\n\n");

    printf("Allocating 3 blocks of 50 bytes each\n\n");
    void *ptr1 = malloc(50);
    void *ptr2 = malloc(50);
    void *ptr3 = malloc(50);
    show_all_chunks_info();

    printf("\nFreeing 0x%lX\n\n", ptr1);
    free(ptr1);
    show_all_chunks_info();
    printf("\nFreeing 0x%lX to create defragmentation\n\n", ptr2);
    free(ptr2);
    show_all_chunks_info();
}

void print_line(void){
    printf("___________________________________________________________________________________\n");
}

int main() {
    srand(time(NULL));

    print_line();
    test_zone(TINY);

    print_line();
    test_zone(SMALL);

    print_line();
    test_zone(LARGE);

    print_line();
    test_mixed_allocations();

    print_line();
    test_realloc();

    print_line();
    test_invalid_pointers();

    print_line();
    test_double_free();

    print_line();
    test_defragmentation();

    print_line();
    return 0;
}
