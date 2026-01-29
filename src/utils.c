#include "malloc.h"

int get_zone_index(void *ptr){
    Header *block_header;

    if (ptr == NULL || g_zones == NULL){
        return ZONE_NOT_FOUND;
    }

    block_header = (Header *) ptr - 1;
    for (int zone_index = 0; zone_index < ZONES_AMOUNT; zone_index++)
    {
        Header *p;
        t_zone *zone = g_zones[zone_index];

        // Zone not initialized
        if (zone->dummy_hdr == NULL || zone->dummy_hdr->s.next == NULL){
            continue;
        }

        p = zone->dummy_hdr->s.next;

        while (p != zone->dummy_hdr){

            if (p == block_header && p->s.is_allocated == true){
                return zone_index;
            }
            p = p->s.next;
        }
    }

    return ZONE_NOT_FOUND;
}

unsigned calculate_units_needed(const size_t size){
    return (size + sizeof(Header) -1) / sizeof(Header) + 1; // +1 for header
}

void print_error_for_object(void *ptr, char *function_name){
    ft_printf("malloc: *** error for object %p: pointer being %s was not allocated\n", ptr, function_name);
}

Header* get_header(void* ptr){
    return (Header*) ptr - 1;
}