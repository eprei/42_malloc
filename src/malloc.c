#include "malloc.h"

static void add_chunk_to_zone(Header *new_chunk, const t_zone *zone){
    Header *ptr;

    for (ptr = zone->dummy_hdr; ptr->s.next != zone->dummy_hdr ; ptr = ptr->s.next){}
    ptr->s.next = new_chunk;
}

// number_of_units is only used for LARGE zones
static Header *add_zone(const int zone_index, const unsigned number_of_units){
    Header *new_chunk;
    char *ptr;
    size_t n_bytes_aligned;
    t_zone *zone = g_zones[zone_index];
    size_t units;

    switch(zone_index){
        case TINY:
            n_bytes_aligned = TINY_ZONE_ALIGNED_SIZE;
            break;
        case SMALL:
            n_bytes_aligned = SMALL_ZONE_ALIGNED_SIZE;
            break;
        case LARGE:
            if (number_of_units * sizeof(Header) / getpagesize() == 0){
                n_bytes_aligned = number_of_units * sizeof(Header);
            } else {
                n_bytes_aligned = (number_of_units * sizeof(Header) / getpagesize() + 1) * getpagesize();
            }
            break;
        default:
            return NULL;
    }

    ptr = mmap(NULL, n_bytes_aligned, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED){
        printf("\t-> add_zone: mmap failed at zone %d\n", zone_index);
        return NULL;
    }

    units = n_bytes_aligned / sizeof(Header);
    new_chunk = (Header *) ptr;
    new_chunk->s.units = units;
    new_chunk->s.next = zone->dummy_hdr;
    new_chunk->s.is_chunk = true;
    new_chunk->s.is_allocated = false;
    add_chunk_to_zone(new_chunk, zone);

    return zone->free_ptr;
}

static void resolve_zone_index(const size_t size, int *zone_index){
    if (size <= TINY_MAX_BYTE_SIZE){
        *zone_index = TINY;
    } else if (size <= SMALL_MAX_BYTE_SIZE){
        *zone_index = SMALL;
    } else {
        *zone_index = LARGE;
    }
}

static bool init_zones(){
    g_zones = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (g_zones == MAP_FAILED){
        printf("init_zones: mmap failed\n");
        return false;
    }

    g_zones[TINY] = (t_zone *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT);
    g_zones[SMALL] = (t_zone *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * SMALL);
    g_zones[LARGE] = (t_zone *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * LARGE);

    g_zones[TINY]->dummy_hdr = (Header *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * ZONES_AMOUNT);
    g_zones[TINY]->free_ptr = NULL;
    g_zones[TINY]->dummy_hdr->s.next = g_zones[TINY]->dummy_hdr;
    g_zones[TINY]->dummy_hdr->s.units = 0;

    g_zones[SMALL]->dummy_hdr = (Header *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * (ZONES_AMOUNT + SMALL));
    g_zones[SMALL]->free_ptr = NULL;
    g_zones[SMALL]->dummy_hdr->s.next = g_zones[SMALL]->dummy_hdr;
    g_zones[SMALL]->dummy_hdr->s.units = 0;

    g_zones[LARGE]->dummy_hdr = (Header *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * (ZONES_AMOUNT + LARGE));
    g_zones[LARGE]->free_ptr = NULL;
    g_zones[LARGE]->dummy_hdr->s.next = g_zones[LARGE]->dummy_hdr;
    g_zones[LARGE]->dummy_hdr->s.units = 0;

    return true;
}

void *malloc(size_t size){
    Header *ptr, *prev_ptr;
    unsigned units_needed;
    t_zone *zone;
    int zone_index;

    if (g_zones == NULL && !init_zones()){
        return NULL;
    }

    resolve_zone_index(size, &zone_index);
    zone = g_zones[zone_index];

    units_needed = (size + sizeof(Header) -1) / sizeof(Header) + 1;
    if ((prev_ptr = zone->free_ptr) == NULL) { /* no free list yet */
        prev_ptr = zone->dummy_hdr;
        zone->free_ptr = zone->dummy_hdr;
        zone->dummy_hdr->s.next = zone->dummy_hdr;
        zone->dummy_hdr->s.units = 0;
    }

    // LARGE zone allocation
    if (zone_index == LARGE){
        ptr = add_zone(zone_index, units_needed);
        if (ptr == NULL){
            return NULL;
        }
        // get last node
        while (ptr->s.next != zone->dummy_hdr){
            ptr = ptr->s.next;
        }
        ptr->s.units = units_needed;
        ptr->s.is_allocated = true;
        ptr->s.size_from_user = size;
        ptr->s.next = zone->dummy_hdr;
        // ptr->s.is_chunk = false;
        printf("ptr allocated at %p in LARGE zone and ptr+1 = %p\n", ptr, ptr + 1);
        return ptr + 1;
    }

    for (ptr = prev_ptr->s.next; ; prev_ptr = ptr, ptr = ptr->s.next) {
        if (ptr->s.units >= units_needed && !ptr->s.is_allocated) { /* big enough */
            // If is not an exact fit, allocate from the tail end
            if (ptr->s.units != units_needed){
                Header *old_next = ptr->s.next;
                ptr->s.units -= units_needed;
                ptr->s.next = ptr + ptr->s.units;
                ptr += ptr->s.units;
                ptr->s.units = units_needed;
                ptr->s.next = old_next;
            }
            ptr->s.size_from_user = size;
            ptr->s.is_allocated = true;
            zone->free_ptr = prev_ptr;
            return ptr + 1;
        }
        if (ptr == zone->free_ptr){
            ptr = add_zone(zone_index, units_needed);
            if (ptr == NULL){
                return NULL;
            }
        }
    }
}
