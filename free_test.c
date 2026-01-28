#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	// write(1, "test 1: free NULL pointer\n",  50);
	// void *ptr = NULL;
	// free(ptr);

	// Malloc prints 'pointer being freed was not allocated' and returns 134
	// write(1, "test 2: free invalid pointer 0xDEADBEEF\n",  50);
	// free((void *)0xDEADBEEF);

	// Malloc prints 'pointer being freed was not allocated' and returns 134
	// write(1, "test 3: free pointer not from malloc\n",  40);
	// int x = 42;
	// free(&x);

	// Malloc prints 'pointer being freed was not allocated' and returns 134
	// write(1, "test 4: double free\n",  22);
    // void *ptr2 = malloc(100);
    // free(ptr2);
	// free(ptr2);

	// Malloc prints 'pointer being freed was not allocated' and returns 134
	// write(1, "test 5: free pointer offset from malloced block\n",  50);
	// void *ptr3 = malloc(100);
	// free((char *)ptr3 + 50);

	return 0;
}
