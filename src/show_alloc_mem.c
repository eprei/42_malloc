#include "malloc.h"
#include "../../../Library/Caches/JetBrains/CLion2025.1/.remote/localhost_2222/19464731-72d4-4f66-9fbc-d5c4a73063e5/usr/lib/gcc/x86_64-linux-gnu/11/include/stdnoreturn.h"

void show_alloc_mem(void){
    size_t bytes_allocated = 0;
    printf("\n\tshow_alloc_mem start\n");
    if (g_zones == NULL){
        printf("No allocations yet\n");
        return;
    }

    for (int i = 0; i < ZONES_AMOUNT; i++){
        t_zone *zone = g_zones[i];
        if (zone->dummy_hdr == NULL || zone->dummy_hdr->s.next == NULL){
            printf("Zone %d: not initialized\n\n", i);
            continue;
        }
        Header *ptr = zone->dummy_hdr->s.next;
        int chunks_founded = 0;
        char *zone_names[] = {"TINY", "SMALL", "LARGE"};

        if (ptr->s.units == 0){
            // printf("is_dummy\n");
            continue;
        }
        printf("%s : 0x%lX\n", zone_names[i], (unsigned long)ptr);

        while (ptr != zone->dummy_hdr){
        // for (ptr = zone->dummy_hdr->s.ptr; ptr != zone->dummy_hdr; ptr = ptr->s.ptr){
            // printf("0x%lX - 0x%lX", (size_t)(void *)ptr, (size_t)(void *)(ptr + ptr->s.units));
            if (ptr->s.is_chunk == true){
                // printf("0x%lX - 0x%lX", (unsigned long)ptr, (unsigned long)(ptr + ptr->s.units));
                // printf("\t-> is chunk\n");
                chunks_founded++;
            }
            // else {
            // }
            if (ptr->s.is_allocated == true){
                printf("0x%lX - 0x%lX", (unsigned long)ptr, (unsigned long)((char *)ptr + ptr->s.size_from_user));
                printf(" : %zu bytes\n", ptr->s.size_from_user);
                bytes_allocated += ptr->s.size_from_user;
            }
            ptr = ptr->s.next;
        }

        printf("%d chunks founded\n", chunks_founded);

        // TODO find out why there's a bug when printing the chunks in the LARGE zone
        // printing format
        // TINY : 0xA0000
        // 0xA0020 - 0xA004A : 42 bytes
        // 0xA006A - 0xA00BE : 84 bytes
        // SMALL : 0xAD000
        // 0xAD020 - 0xADEAD : 3725 bytes
        // LARGE : 0xB0000
        // 0xB0020 - 0xBBEEF : 48847 bytes
        // Total : 52698 bytes
    }
        printf("Total : %lu bytes\n\n", bytes_allocated);
}
