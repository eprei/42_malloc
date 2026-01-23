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

    for (ptr = zone->dummy_hdr; ptr->s.next_free != zone->dummy_hdr ; ptr = ptr->s.next_free){}
    ptr->s.next_free = new_chunk;
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
        printf("\t-> more_core_for_zone: mmap failed at zone %d\n", zone_index);
        return NULL;
    }
    size_t units = n_bytes_aligned / sizeof(Header);
    // printf("**** more_core_for_zone: mmap succeeded for zone %d with %lu bytes and %zu units ****\n", zone_index, n_bytes_aligned, units);

    new_chunk = (Header *) ptr;
    new_chunk->s.size = units;
    // TODO ver si esta linea es correcta
    new_chunk->s.next_free = zone->dummy_hdr;
    new_chunk->s.is_mmaped = 4242;
    add_chunk_to_zone(new_chunk, zone);
    printf("New chunk added to zone %d - address %p - units %zu - is mapped %d\n", zone_index, new_chunk, new_chunk->s.size, new_chunk->s.is_mmaped);

    return zone->free_list;
}

// TODO implement this
static Header *get_ptr_zone_free_list(void *ptr){
    return g_zones[TINY]->free_list;
}

void free(void *ptr){
    // TODO find out to which zone the ptr belongs to or just iterate through all zones?
    Header *block_header_of_ptr, *p;
    Header *freep = get_ptr_zone_free_list(ptr);
    // TODO if ptr is NULL just return

    block_header_of_ptr = (Header *)ptr - 1; /* point to block header */
    for (p = freep; !(block_header_of_ptr > p && block_header_of_ptr < p->s.next_free); p = p->s.next_free)
        if (p >= p->s.next_free && (block_header_of_ptr > p || block_header_of_ptr < p->s.next_free))
            break; /* freed block at start or end of arena */
    if (block_header_of_ptr + block_header_of_ptr->s.size == p->s.next_free) { /* join to upper nbr */
        block_header_of_ptr->s.size += p->s.next_free->s.size;
        block_header_of_ptr->s.next_free = p->s.next_free->s.next_free;
    } else
        block_header_of_ptr->s.next_free = p->s.next_free;
    if (p + p->s.size == block_header_of_ptr) { /* join to lower nbr */
        p->s.size += block_header_of_ptr->s.size;
        p->s.next_free = block_header_of_ptr->s.next_free;
    } else
        p->s.next_free = block_header_of_ptr;
    freep = p;
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
    if (g_zones == MAP_FAILED){
        printf("init_zones: mmap failed\n");
        return false;
    }

    g_zones[TINY] = (t_zone *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT);
    g_zones[SMALL] = (t_zone *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * SMALL);
    g_zones[LARGE] = (t_zone *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * LARGE);

    g_zones[TINY]->dummy_hdr = (Header *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * ZONES_AMOUNT);
    g_zones[TINY]->free_list = NULL;
    g_zones[TINY]->dummy_hdr->s.next_free = g_zones[TINY]->dummy_hdr;
    g_zones[TINY]->dummy_hdr->s.size = 0;

    g_zones[SMALL]->dummy_hdr = (Header *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * (ZONES_AMOUNT + SMALL));
    g_zones[SMALL]->free_list = NULL;
    g_zones[SMALL]->dummy_hdr->s.next_free = g_zones[SMALL]->dummy_hdr;
    g_zones[SMALL]->dummy_hdr->s.size = 0;

    g_zones[LARGE]->dummy_hdr = (Header *)((void *) g_zones + sizeof(t_zone *) * ZONES_AMOUNT + sizeof(t_zone) * (ZONES_AMOUNT + LARGE));
    g_zones[LARGE]->free_list = NULL;
    g_zones[LARGE]->dummy_hdr->s.next_free = g_zones[LARGE]->dummy_hdr;
    g_zones[LARGE]->dummy_hdr->s.size = 0;

    return true;
}

void *malloc(size_t size){
    Header *ptr, *prev_ptr;
    unsigned units_ammount;
    t_zone *zone;
    int zone_index;

    if (g_zones == NULL && !init_zones()){
        return NULL;
    }

    resolve_zone_index(size, &zone_index);
    zone = g_zones[zone_index];

    units_ammount = (size + sizeof(Header) -1) / sizeof(Header) + 1;
    // printf("units_amount = %d\n", units_amount);
    if ((prev_ptr = zone->free_list) == NULL) { /* no free list yet */
        // TODO desglosar la siguiente linea en varias para entender mejor
        // zone->dummy_hdr->s.ptr = zone->free_list = prev_ptr = zone->dummy_hdr;
        prev_ptr = zone->dummy_hdr;
        zone->free_list = zone->dummy_hdr;
        zone->dummy_hdr->s.next_free = zone->dummy_hdr;
        zone->dummy_hdr->s.size = 0;
    }

    // LARGE zone allocation
    if (zone_index == LARGE){
        ptr = add_zone(zone_index, units_ammount);
        if (ptr == NULL){
            return NULL;
        }
        return ptr + 1;
    }

    // TINY and SMALL zones allocation
    for (ptr = prev_ptr->s.next_free; ; prev_ptr = ptr, ptr = ptr->s.next_free) {
        // printf("iterating ptr = %p, prev_ptr = %p, ptr->s.size = %zu, units_amount = %u\n", ptr, prev_ptr, ptr->s.size, units_amount);
        if (ptr->s.size >= units_ammount) { /* big enough */
            // printf("big enough block ptr = %p for zone %d\n", ptr, zone_index);
            if (ptr->s.size == units_ammount){ /* exactly */
                // printf("exactly fitting block ptr = %p for zone %d\n", ptr, zone_index);
                prev_ptr->s.next_free = ptr->s.next_free;
            } else { /* allocate tail end */
                // printf("allocating tail end block ptr = %p for zone %d\n", ptr, zone_index);
                ptr->s.size -= units_ammount;
                ptr += ptr->s.size;
                ptr->s.size = units_ammount;
            }
            zone->free_list = prev_ptr;
            // printf("allocated ptr = %p for zone %d\n", ptr + 1, zone_index);
            return ptr + 1;
        }
        if (ptr == zone->free_list){
            /* wrapped around free list */
            // printf("wrapped around free list for zone %d\n", zone_index);
            ptr = add_zone(zone_index, units_ammount);
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
