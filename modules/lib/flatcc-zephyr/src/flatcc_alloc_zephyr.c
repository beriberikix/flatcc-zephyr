/* SPDX-License-Identifier: Apache-2.0 */

#include "flatcc/flatcc_alloc_zephyr.h"

BUILD_ASSERT(CONFIG_HEAP_MEM_POOL_SIZE > 0,
	     "CONFIG_FLATCC_ALLOC_ZEPHYR requires a kernel heap "
	     "(set CONFIG_HEAP_MEM_POOL_SIZE > 0)");

void *flatcc_zephyr_calloc(size_t nmemb, size_t size)
{
    return k_calloc(nmemb, size);
}

void *flatcc_zephyr_realloc(void *ptr, size_t size)
{
    return k_realloc(ptr, size);
}
