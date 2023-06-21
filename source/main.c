#include "lazypool.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    LazyPool *p = NULL;

    lazypool_create(5, sizeof(int), &p);

    printf("%p\n", lazypool_alloc(p));
    printf("%p\n", lazypool_alloc(p));
    printf("%p\n", lazypool_alloc(p));
    printf("%p\n", lazypool_alloc(p));
    printf("%p\n", lazypool_alloc(p));
    printf("%p\n", lazypool_alloc(p));
    
    lazypool_destroy(&p);

    return 0;
}
