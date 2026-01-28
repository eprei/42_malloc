#include <stdalign.h>
#include <assert.h>

#include "../src/malloc.h"

int main() {
    // write(1, "\nStarting test...\n\n", 18);
    write(1, "\n", 2);

    // printf("\n-> Allocating 3 blocks of 128 bytes each (zone %d)\n", TINY);
    // for (int i = 0; i < 1; i++)
    // {
        // if (i % 10 == 0){
            // printf("i %d : ", i);
        // }
        // char* ptr = malloc(120);
        // ptr[32] = 'A';
        // printf("%c\n", ptr[32]);
    // }
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
    // printf("\n-> Allocating 1 block of 128 bytes each (zone %d)\n", SMALL);
    for (int i = 0; i < 1; i++)
    {
        // if (i % 10 == 0){
            // printf("i %d : ", i);
        // }
        char* ptr2 = malloc(128);
        // ptr2[64] = 'B';
        // printf("%c\n", ptr2[64]);
        free(ptr2);
    }

    printf("\n-> Allocating 1 block of 1025 bytes each (zone %d)\n", LARGE);
    for (int i = 0; i < 1; i++)
    {
        // printf("i %d : ", i);
        char* ptr9 = malloc(1025);
        // ptr9[512] = 'C';
        // printf("%c\n", ptr9[512]);
        free(ptr9);
    }
    void *ptr2 = malloc(100);
    // void *ptr7 = malloc(100);
    // show_alloc_mem();
    show_alloc_mem();
    printf("-> Double free\n");
    printf("ptr2: %p\n", ptr2);
    printf("Freeing ptr2 first time\n");
    free(ptr2);
    /*
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
*/
    printf("-> Freeing pointer offset from malloced block\n");
    // TODO why segfault
    char *ptr77 = malloc(100);
    printf("ptr77: %p\n", ptr77);
    free(ptr77 + 50);

    exit(0);
}
