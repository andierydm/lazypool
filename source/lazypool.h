#ifndef _LAZYPOOL_H_
#define _LAZYPOOL_H_

#include <stddef.h>

typedef struct slot
{
    void **pointer;
    char used;
} Slot;

typedef struct lazypool
{
    Slot *slots;
    size_t slot_size;
    size_t slots_count;
    size_t used_slots_count;
    void *slots_pointers;
} LazyPool;

int lazypool_create(size_t slot_count, size_t slot_size, LazyPool **out_pool);

void *lazypool_alloc(LazyPool *pool);

int lazypool_dealloc(LazyPool *pool, void **pointer);

void lazypool_destroy(LazyPool **pool);

#endif