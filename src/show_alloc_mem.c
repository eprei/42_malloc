#include "malloc.h"
#include "../../../Library/Caches/JetBrains/CLion2025.1/.remote/localhost_2222/19464731-72d4-4f66-9fbc-d5c4a73063e5/usr/lib/gcc/x86_64-linux-gnu/11/include/stdnoreturn.h"

void show_alloc_mem(void){
    size_t bytes_allocated = 0;

    if (g_zones == NULL){
        printf("No allocations yet\n");
        return;
    }

    for (int i = 0; i < ZONES_AMOUNT; i++){
        char *zone_names[] = {"TINY", "SMALL", "LARGE"};
        t_zone *zone = g_zones[i];

        if (zone->dummy_hdr == NULL || zone->dummy_hdr->s.next == NULL){
            printf("Zone %d: not initialized\n\n", i);
            continue;
        }

        Header *ptr = zone->dummy_hdr->s.next;

        if (ptr->s.units == 0){
            continue;
        }

        printf("%s : 0x%lX\n", zone_names[i], (unsigned long)ptr);

        while (ptr != zone->dummy_hdr){
            if (ptr->s.is_allocated == true){
                Header *block_header_ptr = ptr + 1;
                printf("0x%lX - 0x%lX", (unsigned long) block_header_ptr, (unsigned long)((char *)block_header_ptr + ptr->s.size_from_user));
                printf(" : %zu bytes\n", ptr->s.size_from_user);
                bytes_allocated += ptr->s.size_from_user;
            }
            ptr = ptr->s.next;
        }

    }
        printf("Total : %lu bytes\n\n", bytes_allocated);
}
