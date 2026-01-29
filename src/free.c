#include "malloc.h"

static bool all_zones_are_empty(){
    for (int zone_index = 0; zone_index < ZONES_AMOUNT; zone_index++){
        const t_zone *zone = g_zones[zone_index];
        Header *ptr;

        if (zone == NULL || zone->dummy_hdr == NULL || zone->dummy_hdr->s.next == NULL){
            continue;
        }

        ptr = zone->dummy_hdr->s.next;
        while (ptr != zone->dummy_hdr){
            if (ptr->s.is_allocated == true){
                return false;
            }
            ptr = ptr->s.next;
        }
    }

    return true;
}

static void unmap_g_zones(){
    if (all_zones_are_empty()){
        if (munmap(g_zones, getpagesize()) != 0){;
            ft_printf("unmap_g_zones: munmap failed\n");
            return;
        }
        g_zones = NULL;
    }
}

void free(void *ptr){
    Header *block_header, *prev;
    const int zone_index = get_zone_index(ptr);

    if (zone_index == ZONE_NOT_FOUND){
        print_error_for_object(ptr, "freed");
        return;
    }

    block_header = get_header(ptr);
    for (prev = g_zones[zone_index]->dummy_hdr; prev->s.next != block_header; prev = prev->s.next){}

    // If the next block is free, merge with it
    if (block_header->s.next->s.is_allocated == false) {
        Header *next = block_header->s.next;
        block_header->s.units += next->s.units;
        if (next != g_zones[zone_index]->dummy_hdr){
            block_header->s.next = next->s.next;
        }
    }

    // If the previous block is free, merge with it
    if (prev->s.is_allocated == false) {
        if (prev != g_zones[zone_index]->dummy_hdr){
            prev->s.units += block_header->s.units;
        }
        prev->s.next = block_header->s.next;
    }

    block_header->s.is_allocated = false;

    unmap_g_zones();
}
