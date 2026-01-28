#include "malloc.h"

void show_alloc_mem(void){
    size_t bytes_allocated = 0;

    if (g_zones == NULL){
        ft_printf("No allocations yet\n");
        return;
    }

    for (int i = 0; i < ZONES_AMOUNT; i++){
        char *zone_names[] = {"TINY", "SMALL", "LARGE"};
        t_zone *zone = g_zones[i];

        if (zone->dummy_hdr == NULL || zone->dummy_hdr->s.next == NULL){
            ft_printf("Zone %d: not initialized\n\n", i);
            continue;
        }

        Header *ptr = zone->dummy_hdr->s.next;

        if (ptr->s.units == 0){
            continue;
        }

        ft_printf("%s : %p\n", zone_names[i], (unsigned long)ptr);

        while (ptr != zone->dummy_hdr){
            if (ptr->s.is_allocated == true){
                Header *block_header_ptr = ptr + 1;
                ft_printf("%p - %p", (unsigned long) block_header_ptr, (unsigned long)((char *)block_header_ptr + ptr->s.size_from_user));
                // printf(" : %zu bytes\n", ptr->s.size_from_user);
                ft_printf(" : %z bytes\n", ptr->s.size_from_user);
                bytes_allocated += ptr->s.size_from_user;
            }
            ptr = ptr->s.next;
        }

    }
        ft_printf("Total : %z bytes\n\n", bytes_allocated);
}
