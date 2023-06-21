#include "lazypool.h"
#include "lazypool_codes.h"
#include <stdlib.h>
#include <string.h>

void *get_pointer(size_t index, size_t slot_size, void *slots)
{
    return (slots + (slot_size * index));
}

int lazypool_create(size_t slots_count, size_t slot_size, LazyPool **out_pool)
{
    int code = -1;
    Slot *slots = NULL;
    void *slots_pointers = NULL;
    LazyPool *pool = NULL;

    slots = malloc(sizeof(Slot) * slots_count);

    if (!slots)
    {
        code = LAZYPOOL_FAILED_ALLOC;
        goto CLEAN_UP;
    }

    slots_pointers = calloc(slots_count, slot_size);

    if (!slots_pointers)
    {
        code = LAZYPOOL_FAILED_ALLOC;
        goto CLEAN_UP;
    }

    pool = malloc(sizeof(LazyPool));

    if (!pool)
    {
        code = LAZYPOOL_FAILED_ALLOC;
        goto CLEAN_UP;
    }

    size_t counter = 0;

    while (counter < slots_count)
    {
        Slot *slot = get_pointer(counter, sizeof(Slot), slots);
        void *slot_pointer = get_pointer(counter, slot_size, slots_pointers);

        slot->pointer = slot_pointer;
        slot->used = 0;

        counter++;
    }

    pool->slots = slots;
    pool->slot_size = slot_size;
    pool->slots_count = slots_count;
    pool->slots_pointers = slots_pointers;
    pool->used_slots_count = 0;

    *out_pool = pool;

    return LAZYPOOL_OK;

CLEAN_UP:
    free(pool);
    free(slots);
    free(slots_pointers);

    return code;
}

void *lazypool_alloc(LazyPool *pool)
{
    if (pool->used_slots_count == pool->slots_count)
        return NULL;

    size_t counter = 0;

    while (counter < pool->slots_count)
    {
        Slot *slot = get_pointer(counter, sizeof(Slot), pool->slots);

        if (!slot->used)
        {
            pool->used_slots_count++;
            slot->used = 1;

            return slot->pointer;
        }

        counter++;
    }

    return NULL;
}

int lazypool_dealloc(LazyPool *pool, void **pointer)
{
    if (pool->used_slots_count == 0)
        return LAZYPOOL_INVALID_POINTER;

    size_t counter = 0;

    while (counter < pool->slots_count)
    {
        Slot *slot = get_pointer(counter, sizeof(Slot), pool->slots);

        if (slot->pointer == *pointer)
        {
            memset(slot->pointer, 0, pool->slot_size);
            slot->used = 0;
            pool->used_slots_count--;

            return LAZYPOOL_OK;
        }

        counter++;
    }

    *pointer = NULL;

    return LAZYPOOL_OK;
}

void lazypool_destroy(LazyPool **pool)
{
    if (!pool || !*pool)
        return;

    free((*pool)->slots_pointers);
    free((*pool)->slots);
    free(*pool);
    *pool = NULL;
}