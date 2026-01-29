#ifndef MALLOC_H
# define MALLOC_H

# include "../libft/libft.h"
# include <unistd.h>
# include <sys/mman.h>
# include <stddef.h>
# include <stdbool.h>

# define TINY_MAX_BYTE_SIZE    128                                                                                    // n
# define SMALL_MAX_BYTE_SIZE   1024                                                                                   // m

# define MINIMAL_ALLOCATIONS_REQUIRED 100
# define UNIT_SIZE sizeof(Header)

# define TINY_ZONE_MIN_REQUIRED_SIZE ((sizeof(Header) + TINY_MAX_BYTE_SIZE) * MINIMAL_ALLOCATIONS_REQUIRED)           // not multiple of getpagesize()
# define SMALL_ZONE_MIN_REQUIRED_SIZE ((sizeof(Header) + SMALL_MAX_BYTE_SIZE) * MINIMAL_ALLOCATIONS_REQUIRED)         // not multiple of getpagesize()

# define TINY_ZONE_ALIGNED_SIZE ((TINY_ZONE_MIN_REQUIRED_SIZE / getpagesize()) + 1) * getpagesize()                    // N, multiple of getpagesize()
# define SMALL_ZONE_ALIGNED_SIZE ((SMALL_ZONE_MIN_REQUIRED_SIZE / getpagesize()) + 1) * getpagesize()                  // M, multiple of getpagesize()

# define UNITS_IN_TINY_ZONE (TINY_ZONE_ALIGNED_SIZE / UNIT_SIZE)
# define UNITS_IN_SMALL_ZONE (SMALL_ZONE_ALIGNED_SIZE / UNIT_SIZE)

// Zones
# define TINY 0
# define SMALL 1
# define LARGE 2
# define ZONES_AMOUNT 3
# define ZONE_NOT_FOUND -1
# define DUMMY_HEADER 0

// Header is also the allocation unit
typedef union header {
    struct {
        union header *next;
        size_t units;
        size_t size_from_user;
        bool is_chunk; // TODO delete when done testing
        bool is_allocated;
    } s;
    max_align_t align; // Forcing alignment of Units using the most restrictive alignment type
} Header;

typedef struct s_zone{
    Header *dummy_hdr;
    Header *free_ptr;
} t_zone;

t_zone **g_zones;

// Mandatory exposed functions
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void show_alloc_mem(void);

// Utility functions for internal use
int get_zone_index(void *ptr);
unsigned calculate_units_needed(size_t size);
void print_error_for_object(void *ptr, char *function_name);
Header* get_header(void* ptr);

#endif
