#include <zephyr/kernel.h>
#include <string.h>

#include "flatcc/flatcc_builder.h"

/*
 * Test FlatCC runtime integration with Zephyr.
 *
 * Without schema-generated code we exercise the raw builder/emitter:
 *   1. Initialise flatcc_builder (triggers allocator)
 *   2. Create a buffer containing a struct (Vec3: 3 × float)
 *   3. Retrieve the serialised bytes via the default emitter
 *   4. Verify we got a non-empty buffer
 *   5. Tear down the builder (releases all allocations)
 */

/* A trivial 12-byte struct: three little-endian floats. */
struct vec3 {
    float x;
    float y;
    float z;
};

int main(void)
{
    flatcc_builder_t builder;
    flatcc_builder_t *B = &builder;
    int ret;

    ret = flatcc_builder_init(B);
    if (ret) {
        printk("FAIL: flatcc_builder_init returned %d\n", ret);
        return 1;
    }
    printk("builder initialised (allocator OK)\n");

    /* Open a buffer (no file-identifier, default alignment, no flags). */
    ret = flatcc_builder_start_buffer(B, 0, 0, 0);
    if (ret) {
        printk("FAIL: start_buffer returned %d\n", ret);
        flatcc_builder_clear(B);
        return 1;
    }

    /* Create a struct as the root object. */
    struct vec3 v = { 1.5f, 2.5f, 3.5f };
    flatcc_builder_ref_t root =
        flatcc_builder_create_struct(B, &v, sizeof(v), sizeof(float));

    /* Close the buffer with the struct as root. */
    flatcc_builder_ref_t buf_ref = flatcc_builder_end_buffer(B, root);
    if (!buf_ref) {
        printk("FAIL: end_buffer returned 0\n");
        flatcc_builder_clear(B);
        return 1;
    }

    /* Read back the serialised bytes from the default emitter. */
    size_t size = 0;
    void *buf = flatcc_builder_get_direct_buffer(B, &size);
    if (!buf || size == 0) {
        printk("FAIL: get_direct_buffer returned NULL or zero size\n");
        flatcc_builder_clear(B);
        return 1;
    }

    printk("created %u-byte FlatBuffer with Vec3 struct\n", (unsigned)size);

    /* Quick sanity: the buffer must be at least header(4) + struct(12). */
    if (size >= 16) {
        printk("PASS\n");
    } else {
        printk("FAIL: buffer too small (%u bytes)\n", (unsigned)size);
        flatcc_builder_clear(B);
        return 1;
    }

    flatcc_builder_clear(B);
    printk("builder cleared — all memory freed\n");

    return 0;
}
