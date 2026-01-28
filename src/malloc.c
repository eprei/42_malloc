#include "malloc.h"

// static Header *more_core_for_zone(unsigned nu){
//     write(1, "morecore called\n", 16);
//
//     char *ptr;
//     Header *header_ptr;
//     if (nu < NALLOC){
//         nu = NALLOC;
//     }
//     ptr = mmap(NULL, nu * sizeof(Header), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//     if (ptr == MAP_FAILED){
//         printf("morecore: mmap failed\n");
//         return NULL;
//     }
//
//     header_ptr = (Header *) ptr;
//     header_ptr->s.size = nu;
//     free(header_ptr + 1);
//     return freep;
// }
//
// static void add_chunk_to_free_list(Header *new_chunk, t_zone *zone){
//     Header *ptr;
//
//     for (ptr = zone->free_list; !(new_chunk > ptr && new_chunk < ptr->s.ptr); ptr = ptr->s.ptr)
//         if (ptr >= ptr->s.ptr && (new_chunk > ptr || new_chunk < ptr->s.ptr))
//             break; /* freed block at start or end of arena */
//     if (new_chunk + new_chunk->s.size == ptr->s.ptr) { /* join to upper nbr */
//         new_chunk->s.size += ptr->s.ptr->s.size;
//         new_chunk->s.ptr = ptr->s.ptr->s.ptr;
//     } else
//         new_chunk->s.ptr = ptr->s.ptr;
//     if (ptr + ptr->s.size == new_chunk) { /* join to lower nbr */
//         ptr->s.size += new_chunk->s.size;
//         ptr->s.ptr = new_chunk->s.ptr;
//     } else
//         ptr->s.ptr = new_chunk;
//     zone->free_list = ptr;
// }


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

    switch(zone_index){
        case TINY:
            n_bytes_aligned = TINY_ZONE_ALIGNED_SIZE;
            // printf("TINY_ZONE_ALIGNED_SIZE = %lu\n", n_bytes_aligned);
            break;
        case SMALL:
            n_bytes_aligned = SMALL_ZONE_ALIGNED_SIZE;
            // printf("SMALL_ZONE_ALIGNED_SIZE = %lu\n", n_bytes_aligned);
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
    size_t units = n_bytes_aligned / sizeof(Header);
    // printf("**** more_core_for_zone: mmap succeeded for zone %d with %lu bytes and %zu units ****\n", zone_index, n_bytes_aligned, units);

    new_chunk = (Header *) ptr;
    new_chunk->s.units = units;
    new_chunk->s.next = zone->dummy_hdr;
    // printf("New chunk created at zone %d - address 0x%lX - units %zu\n", zone_index, (unsigned long)new_chunk, new_chunk->s.units);
    new_chunk->s.is_chunk = true;
    new_chunk->s.is_allocated = false;
    add_chunk_to_zone(new_chunk, zone);
    // printf("New chunk added to zone %d - address 0x%lX - units %zu\n", zone_index, (unsigned long)new_chunk, new_chunk->s.units);

    return zone->free_ptr;
}

// void free(void *ptr){
//     Header *block_header_ptr, *p;
//     Header *freep;
//
//     block_header_ptr = ptr - 1;
//     for (p = freep; !(block_header_ptr > p && block_header_ptr < p->s.next); p = p->s.next){
//         if (p >= p->s.next && (block_header_ptr > p || block_header_ptr < p->s.next))
//             break; /* freed block at start or end of arena */
//     }
//
//     if (block_header_ptr + block_header_ptr->s.units == p->s.next) { /* join to upper nbr */
//         block_header_ptr->s.units += p->s.next->s.units;
//         block_header_ptr->s.next = p->s.next->s.next;
//     } else {
//         block_header_ptr->s.next = p->s.next;
//     }
//     if (p + p->s.units == block_header_ptr) { /* join to lower nbr */
//         p->s.units += block_header_ptr->s.units;
//         p->s.next = block_header_ptr->s.next;
//     } else {
//         p->s.next = block_header_ptr;
//     }
//     freep = p;
// }

// TODO substitute all printf with ft_printf
static void print_error_for_object(void *ptr){
    printf("malloc: *** error for object %p: pointer being freed was not allocated\n", ptr);
}

static int validate_pointer(void *ptr){
    Header *block_header;
    // printf("validate_pointer: validating pointer %p\n", ptr);
    if (ptr == NULL || g_zones == NULL){
        // printf("validate_pointer: pointer is NULL or g_zones is NULL\n");
        return ZONE_NOT_FOUND;
    }

    // printf("validate_pointer: traversing zones to find pointer %p\n", ptr);
    block_header = (Header *) ptr - 1;
    for (int zone_index = 0; zone_index < ZONES_AMOUNT; zone_index++)
    {
        Header *p;
        // printf("validate_pointer: checking zone %d\n", zone_index);
        t_zone *zone = g_zones[zone_index];

        // Zone not initialized
        if (zone->dummy_hdr == NULL || zone->dummy_hdr->s.next == NULL){
            // printf("validate_pointer: zone %d not initialized, continuing\n", zone_index);
            continue;
        }

        // printf("validate_pointer: zone %d dummy header %p\n", zone_index, zone->dummy_hdr);
        p = zone->dummy_hdr->s.next;
        // printf("validate_pointer: starting to traverse zone %d from block %p\n", zone_index, p);


        while (p != zone->dummy_hdr){
            // printf("validate_pointer: checking block %p in zone %d\n", p, zone_index);
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
    // printf("all_zones_are_empty: checking all zones\n");
    for (int zone_index = 0; zone_index < ZONES_AMOUNT; zone_index++){
        const t_zone *zone = g_zones[zone_index];
        Header *ptr;

        // printf("all_zones_are_empty: checking zone %s\n", zone_names[zone_index]);
        if (zone == NULL || zone->dummy_hdr == NULL || zone->dummy_hdr->s.next == NULL){
            printf("all_zones_are_empty: zone %s not initialized or empty, continuing\n", zone_names[zone_index]);
            continue;
        }

        // printf("all_zones_are_empty: traversing zone %s\n", zone_names[zone_index]);

        // TODO resolve bug
        ptr = zone->dummy_hdr->s.next;
        while (ptr != zone->dummy_hdr){
            // printf("all_zones_are_empty: checking block %p in zone %s\n", ptr,  zone_names[zone_index]);
            if (ptr->s.is_allocated == true){
                return false;
            }
            // printf("p->s.next = %p\n", ptr->s.next);
            ptr = ptr->s.next;
        }
        printf("zone %s is empty\n", zone_names[zone_index]);
    }

    return true;
}

// TODO seguir aca hay un problema cuando se libera todo el espacio y se intenta volver a asignar
// se deberia crear una nueva zona por que la otra deberia estar borrada (?)
static void unmap_g_zones(){
    // printf("unmap_g_zones: checking if all zones are empty\n");
    if (all_zones_are_empty()){
        // printf("unmap_g_zones: all zones are empty, unmapping g_zones\n");
        if (munmap(g_zones, getpagesize()) != 0){;
            printf("unmap_g_zones: munmap failed\n");
            return;
        }
        printf("unmap_g_zones: munmap succeeded: %p\n", g_zones);
        g_zones = NULL;
        printf("g_zones: %p\n", g_zones);
    }
}

void free(void *ptr){
    // printf("free: called with pointer %p\n", ptr);
    char *zone_names[] = {"TINY", "SMALL", "LARGE"};
    Header *block_header_ptr, *prev;
    // Header *freep;
    const int zone_index = validate_pointer(ptr);

    // printf("free: called with pointer %p\n", ptr);
    if (zone_index == ZONE_NOT_FOUND){
        // printf("free: invalid pointer %p, aborting free\n", ptr);
        return;
    }

    print_dummy(zone_index);
    // freep = g_zones[zone_index]->free_ptr;
    // printf("free: found zone %d for pointer %p\n", zone_index, ptr);
    // block_header_ptr = (Header *) ptr - 1;
    // for (p = freep; !(block_header_ptr > p && block_header_ptr < p->s.next); p = p->s.next){
    //     printf("free: traversing free list, current block %p\n", p);
    //     if (p >= p->s.next && (block_header_ptr > p || block_header_ptr < p->s.next)){
    //         break; /* freed block at start or end of arena */
    //     }
    //     sleep(1);
    // }

    // printf("free: found zone %d for pointer %p\n", zone_index, ptr);
    block_header_ptr = (Header *) ptr - 1;
    for (prev = g_zones[zone_index]->dummy_hdr; prev->s.next != block_header_ptr; prev = prev->s.next){
        // printf("free: traversing list %s drom dummy_hdr, current block %p\n", zone_names[zone_index], prev);
        // if (p >= p->s.next && (block_header_ptr > p || block_header_ptr < p->s.next)){
            // break; /* freed block at start or end of arena */
        // }
        // sleep(1);
    }

    // printf("Freeing pointer %p and header %p in zone %d\n", ptr, block_header_ptr, zone_index);
    // If the next block is free, merge with it
    if (block_header_ptr->s.next->s.is_allocated == false) {
        // printf("Merging with NEXT free block at %p\n", block_header_ptr->s.next);

        Header *next = block_header_ptr->s.next;
        block_header_ptr->s.units += next->s.units;
        if (next != g_zones[zone_index]->dummy_hdr){
            block_header_ptr->s.next = next->s.next;
        }
        // next->s.next = NULL;
    }
    // If the previous block is free, merge with it
    if (prev->s.is_allocated == false) {
        // printf("Merging with PREVIOUS free block at %p\n", prev);

        if (prev != g_zones[zone_index]->dummy_hdr){
            prev->s.units += block_header_ptr->s.units;
        }
        prev->s.next = block_header_ptr->s.next;
        // block_header_ptr->s.is_allocated = false;
        // block_header_ptr->s.next = NULL;
    }

    block_header_ptr->s.is_allocated = false;
    print_dummy(zone_index);


    // freep = p; // para que sirve esto?
    unmap_g_zones();
}

static void resolve_zone_index(const size_t size, int *zone_index){
    if (size <= TINY_MAX_BYTE_SIZE){
        // printf("get_zone_index: TINY zone selected for size %zu\n", size);
        *zone_index = TINY;
    } else if (size <= SMALL_MAX_BYTE_SIZE){
        // printf("get_zone_index: SMALL zone selected for size %zu\n", size);
        *zone_index = SMALL;
    } else {
        // printf("get_zone_index: LARGE zone selected for size %zu\n", size);
        *zone_index = LARGE;
    }
}

static bool init_zones(){
    g_zones = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    printf("init_zones: initializing g_zones at %p\n", g_zones);
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

    // printf("malloc: requested size %zu\n", size);
    if (g_zones == NULL && !init_zones()){
        return NULL;
    }

    // printf("malloc: resolving zone for size %zu\n", size);
    resolve_zone_index(size, &zone_index);
    zone = g_zones[zone_index];

    // printf("malloc: allocating in zone %d\n", zone_index);
    units_needed = (size + sizeof(Header) -1) / sizeof(Header) + 1;
    // printf("units_amount = %d\n", units_amount);
    // printf("malloc: units needed %u\n", units_needed);
    if ((prev_ptr = zone->free_ptr) == NULL) { /* no free list yet */
        // TODO desglosar la siguiente linea en varias para entender mejor
        // zone->dummy_hdr->s.ptr = zone->free_list = prev_ptr = zone->dummy_hdr;
        printf("===> malloc: initializing free list for zone %d\n", zone_index);
        prev_ptr = zone->dummy_hdr;
        zone->free_ptr = zone->dummy_hdr;
        zone->dummy_hdr->s.next = zone->dummy_hdr;
        zone->dummy_hdr->s.units = 0;
        // printf("zone->dummy_hdr = %p\n", zone->dummy_hdr);
        // printf("prev_ptr = %p\n", prev_ptr);
        // printf("zone->dummy_hdr->s.next = %p\n", zone->dummy_hdr->s.next);
        // printf("prev_ptr.s.next = %p\n", prev_ptr->s.next);
    }

    // printf("malloc: starting search for free block in zone %d\n", zone_index);
    // LARGE zone allocation
    if (zone_index == LARGE){
        ptr = add_zone(zone_index, units_needed);
        if (ptr == NULL){
            return NULL;
        }
        while (ptr->s.next != zone->dummy_hdr){
            ptr = ptr->s.next;
        }
        ptr->s.units = units_needed;
        ptr->s.is_allocated = true;
        ptr->s.size_from_user = size;
        ptr->s.next = zone->dummy_hdr;
        // ptr->s.is_chunk = false;
        return ptr + 1;
    }

    printf("malloc: searching for free block in TINY/SMALL zone %d\n", zone_index);
    // TINY and SMALL zones allocation
    // printf("malloc: prev_ptr = %p\n", prev_ptr);
    // if (ptr->s.next == NULL)
    // {
    //     printf("malloc: ptr.s.next is NULL, something went wrong\n");
    //     return NULL;
    // }
    // if (prev_ptr == NULL)
    // {
    //     printf("malloc: prev_ptr is NULL, something went wrong\n");
    //     return NULL;
    // }
    // if (prev_ptr->s.next == NULL)
    // {
    //     printf("malloc: prev_ptr.s.next is NULL, something went wrong\n");
    //     return NULL;
    // }
    // printf("ptr: %p\n", ptr);
    // printf("prev_ptr->s.next = %p\n", prev_ptr->s.next);
    for (ptr = prev_ptr->s.next; ; prev_ptr = ptr, ptr = ptr->s.next) {
        // printf("malloc: checking block %p with %zu units (allocated: %d) in zone %d\n", ptr, ptr->s.units, ptr->s.is_allocated, zone_index);
        if (ptr->s.units >= units_needed && !ptr->s.is_allocated) { /* big enough */
            // printf("big enough block ptr = %p for zone %d\n", ptr, zone_index);
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
            // printf("allocated ptr = %p for zone %d\n", ptr + 1, zone_index);
            return ptr + 1;
        }
        if (ptr == zone->free_ptr){
            /* wrapped around free list */
            // printf("wrapped around free list for zone %d\n", zone_index);
            ptr = add_zone(zone_index, units_needed);
            if (ptr == NULL){
                return NULL;
            }
        }
    }

    // units_amount = (size + sizeof(Header) -1) / sizeof(Header) + 1;
    // if ((prevp = freep) == NULL) { /* no free list yet */
    //     base.s.ptr = freep = prevp = &base;
    //     base.s.size = 0;
    // }
    // for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
    //     if (p->s.size >= units_amount) { /* big enough */
    //         if (p->s.size == units_amount) /* exactly */
    //             prevp->s.ptr = p->s.ptr;
    //         else { /* allocate tail end */
    //             p->s.size -= units_amount;
    //             p += p->s.size;
    //             p->s.size = units_amount;
    //         }
    //         freep = prevp;
    //         return p + 1;
    //     }
    //     if (p == freep) /* wrapped around free list */
    //         if ((p = morecore(units_amount)) == NULL)
    //             return NULL; /* none left */
    // }
}
