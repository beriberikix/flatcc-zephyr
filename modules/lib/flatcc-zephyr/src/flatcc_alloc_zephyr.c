#include "flatcc/flatcc_alloc_zephyr.h"

void *flatcc_zephyr_calloc(size_t nmemb, size_t size)
{
    return k_calloc(nmemb, size);
}

void *flatcc_zephyr_realloc(void *ptr, size_t size)
{
    return k_realloc(ptr, size);
}
