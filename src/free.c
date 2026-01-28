#include "malloc.h"

// TODO substitute all printf with ft_printf
static void print_error_for_object(void *ptr){
    ft_printf("malloc: *** error for object %p: pointer being freed was not allocated\n", ptr);
}

static int validate_pointer(void *ptr){
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

    print_error_for_object(ptr);
    return ZONE_NOT_FOUND;
}

static bool all_zones_are_empty(){
    char *zone_names[] = {"TINY", "SMALL", "LARGE"};

    for (int zone_index = 0; zone_index < ZONES_AMOUNT; zone_index++){
        const t_zone *zone = g_zones[zone_index];
        Header *ptr;

        if (zone == NULL || zone->dummy_hdr == NULL || zone->dummy_hdr->s.next == NULL){
            // ft_printf("all_zones_are_empty: zone %s not initialized or empty, continuing\n", zone_names[zone_index]);
            continue;
        }

        ptr = zone->dummy_hdr->s.next;
        while (ptr != zone->dummy_hdr){
            if (ptr->s.is_allocated == true){
                return false;
            }
            ptr = ptr->s.next;
        }
        // ft_printf("zone %s is empty\n", zone_names[zone_index]);
    }

    return true;
}

static void unmap_g_zones(){
    if (all_zones_are_empty()){
        if (munmap(g_zones, getpagesize()) != 0){;
            ft_printf("unmap_g_zones: munmap failed\n");
            return;
        }
        ft_printf("unmap_g_zones: munmap succeeded: %p\n", g_zones);
        g_zones = NULL;
        ft_printf("g_zones: %p\n", g_zones);
    }
}

void free(void *ptr){
    Header *block_header_ptr, *prev;
    const int zone_index = validate_pointer(ptr);

    if (zone_index == ZONE_NOT_FOUND){
        return;
    }

    block_header_ptr = (Header *) ptr - 1;
    for (prev = g_zones[zone_index]->dummy_hdr; prev->s.next != block_header_ptr; prev = prev->s.next){}

    // If the next block is free, merge with it
    if (block_header_ptr->s.next->s.is_allocated == false) {
        Header *next = block_header_ptr->s.next;
        block_header_ptr->s.units += next->s.units;
        if (next != g_zones[zone_index]->dummy_hdr){
            block_header_ptr->s.next = next->s.next;
        }
    }

    // If the previous block is free, merge with it
    if (prev->s.is_allocated == false) {
        if (prev != g_zones[zone_index]->dummy_hdr){
            prev->s.units += block_header_ptr->s.units;
        }
        prev->s.next = block_header_ptr->s.next;
    }

    block_header_ptr->s.is_allocated = false;

    unmap_g_zones();
}