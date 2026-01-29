#include "malloc.h"

static bool change_alloc_size(Header *block_header, const size_t size){
    const unsigned units_needed = calculate_units_needed(size);
    unsigned units_available = block_header->s.units;
    bool there_is_space = false;
    Header *current_header;

    current_header = block_header->s.next;
    while (!current_header->s.is_allocated && !there_is_space && current_header->s.units != DUMMY_HEADER){
        units_available += current_header->s.units;
        current_header = current_header->s.next;
        there_is_space = units_available >= units_needed;

        if (there_is_space){
            block_header->s.units = units_available;
            block_header->s.size_from_user = size;
            block_header->s.next = current_header;
        }
    }

    return there_is_space;
}


void *realloc(void *ptr, size_t size){
    Header *block_header;

    if (ptr == NULL){
        return malloc(size);
    }

    if (size == 0){
        free(ptr);
        return NULL;
    }

    if (get_zone_index(ptr) == ZONE_NOT_FOUND){
        print_error_for_object(ptr, "realloc'd");
        return NULL;
    }

    block_header = get_header(ptr);

    if (size < block_header->s.size_from_user){
        block_header->s.size_from_user = size;
        return ptr;
    }

    if (!change_alloc_size(block_header, size)){
        void* new_ptr = malloc(size);
        ft_memcpy(new_ptr, ptr, block_header->s.size_from_user);
        free(ptr);
        return new_ptr;
    }

    return ptr;
}
