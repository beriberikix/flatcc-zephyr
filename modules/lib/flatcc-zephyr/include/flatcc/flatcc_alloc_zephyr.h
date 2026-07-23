/* SPDX-License-Identifier: Apache-2.0 */

/**
 * @file flatcc_alloc_zephyr.h
 * @brief Zephyr kernel-heap allocator shim for the FlatCC runtime.
 *
 * @defgroup flatcc_alloc_zephyr Zephyr allocator shim
 * @brief Maps the FlatCC runtime's allocation macros onto the Zephyr
 *        kernel heap.
 *
 * When @c CONFIG_FLATCC_ALLOC_ZEPHYR is enabled (the default), the module
 * build redefines @c FLATCC_ALLOC / @c FLATCC_FREE / @c FLATCC_CALLOC /
 * @c FLATCC_REALLOC to @c k_malloc / @c k_free and the two wrappers below
 * for every translation unit of the @c flatccrt library. A kernel heap is
 * required (@c CONFIG_HEAP_MEM_POOL_SIZE > 0, enforced at compile time).
 *
 * @note The override is scoped to the @c flatccrt library only:
 *       application code that invokes FlatCC allocation macros directly
 *       still gets the libc allocator.
 * @{
 */

#ifndef FLATCC_ALLOC_ZEPHYR_H
#define FLATCC_ALLOC_ZEPHYR_H

#include <stddef.h>
#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate zero-initialized memory from the Zephyr kernel heap.
 *
 * Wraps @c k_calloc with libc @c calloc semantics for the FlatCC
 * runtime's @c FLATCC_CALLOC macro.
 *
 * @param nmemb Number of elements.
 * @param size  Size of one element in bytes.
 * @return Pointer to zeroed memory, or NULL on exhaustion or overflow of
 *         `nmemb * size`.
 */
void *flatcc_zephyr_calloc(size_t nmemb, size_t size);

/**
 * @brief Resize an allocation on the Zephyr kernel heap.
 *
 * Wraps @c k_realloc with libc @c realloc semantics for the FlatCC
 * runtime's @c FLATCC_REALLOC macro: a NULL @p ptr allocates, a zero
 * @p size frees, and on failure the original allocation is left intact.
 *
 * @param ptr  Existing allocation from this shim/@c k_malloc, or NULL.
 * @param size New size in bytes.
 * @return Pointer to the resized memory, or NULL on failure.
 */
void *flatcc_zephyr_realloc(void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* FLATCC_ALLOC_ZEPHYR_H */

/** @} */ /* flatcc_alloc_zephyr */
