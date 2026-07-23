/* SPDX-License-Identifier: Apache-2.0 */

#ifndef FLATCC_ALLOC_ZEPHYR_H
#define FLATCC_ALLOC_ZEPHYR_H

#include <stddef.h>
#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

void *flatcc_zephyr_calloc(size_t nmemb, size_t size);
void *flatcc_zephyr_realloc(void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* FLATCC_ALLOC_ZEPHYR_H */
