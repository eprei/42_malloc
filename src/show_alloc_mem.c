#include "malloc.h"

void show_alloc_mem(void){
    for (int i = 0; i < ZONES_AMOUNT; i++){
        t_zone *zone = g_zones[i];
        Header *ptr = zone->dummy_hdr->s.next_free;
        int count = 0;

        if (zone->dummy_hdr == NULL){
            printf("Zone %d: not initialized\n\n", i);
            continue;
        }
        char *zone_names[] = {"TINY", "SMALL", "LARGE"};
        printf("\n%s : %p\n", zone_names[i], zone->dummy_hdr->s.next_free);
        if (ptr->s.size== 0){
            printf("is_dummy\n");
        }

        while (ptr != zone->dummy_hdr){
        // for (ptr = zone->dummy_hdr->s.ptr; ptr != zone->dummy_hdr; ptr = ptr->s.ptr){
            printf("0x%lX - 0x%lX", (size_t)(void *)ptr, (size_t)(void *)(ptr + ptr->s.size));
            if (ptr->s.is_mmaped == 4242){
                printf("\t-> this chunk is mmaped\n");
                count++;
            }
            ptr = ptr->s.next_free;
        }

        printf("%d chunks founded\n", count);
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
}
