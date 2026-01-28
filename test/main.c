#include <stdalign.h>
#include <assert.h>
#include <time.h>

#include "../src/malloc.h"

int main() {
    // write(1, "\nStarting test...\n\n", 18);
    write(1, "\n", 2);
    show_alloc_mem();

    // printf("\n-> Allocating 3 blocks of 128 bytes each (zone %d)\n", TINY);
    // for (int i = 0; i < 1; i++)
    // {
        // if (i % 10 == 0){
            // printf("i %d : ", i);
        // }
        // char* ptr = malloc(120);
        // ptr[32] = 'A';
        // printf("ptr = %p and char = %c\n", ptr, ptr[32]);
    // }
    // show_alloc_mem();
        // void* ptr = malloc(1);
        // ptr = malloc(2);
        // ptr = malloc(3);

        // ptr = malloc(130);
        // ptr = malloc(131);
        // void *ptr = malloc(132);


        // ptr = malloc(1025);
        // ptr = malloc(1026);
        // ptr = malloc(1027);

        // ptr = malloc(84);
        // ptr = malloc(200);
        // ptr = malloc(48847);
        // ptr = malloc(48847);
    srand(time(NULL));   // Initialization, should only be called once.
    printf("\n-> Allocating 3 block of 100 bytes each (zone %d)\n", TINY);
    for (int i = 0; i < 3; i++)
    {
        // if (i % 10 == 0){
            // printf("i %d : ", i);
        // }
        char* ptr2 = malloc(100);
        printf("ptr2 address: %p\n", ptr2);
        // int r = rand() % 99;
        // ptr2[r] = 'B';
        // printf("%c\n", ptr2[r]);
        // free(ptr2);
    }
    show_alloc_mem();

    printf("\n-> Allocating 3 blocs of 1025 bytes each (zone %d)\n", LARGE);
    for (int i = 0; i < 3; i++)
    {
        // printf("i %d : ", i);
        char* ptr9 = malloc(1025);
        // ptr9[1000] = 'C';
        // printf("ptr = %p and char = %c\n", ptr9, ptr9[1000]);
        // free(ptr9);
    }
    show_alloc_mem();
    /*
    printf("\n-> Allocating double free test in small zone\n");
    void *ptr2 = malloc(200);
    // void *ptr7 = malloc(100);
    // show_alloc_mem();
    printf("ptr2: %p\n", ptr2);
    show_alloc_mem();
    printf("Freeing ptr2 first time\n");
    free(ptr2);
    printf("Freeing ptr2 second time\n");
    free(ptr2);
    show_alloc_mem();

    printf("\n-> Freeing NULL pointer\n");
    void *ptr = NULL;
    free(ptr);
    //
    printf("-> Freeing invalid pointer 0xDEADBEEF\n");
    free((void *)0xDEADBEEF);
    //
    printf("-> Freeing pointer not from malloc\n");
    int x = 42;
    free(&x);
    //
    printf("-> Freeing pointer offset from malloced block\n");
    char *ptr77 = malloc(100);
    printf("ptr77: %p\n", ptr77);
    free(ptr77 + 50);
    exit(0);
    */
}
