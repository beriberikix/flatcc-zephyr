# flatcc basic sample

Minimal Zephyr app that exercises the raw FlatCC builder/emitter API without
schema-generated code.

## What it tests

- Module discovery through `EXTRA_ZEPHYR_MODULES`
- Runtime library target creation and linking (`flatccrt`)
- `flatcc_builder_init` / `flatcc_builder_clear` (exercises Zephyr allocators)
- `flatcc_builder_create_struct` and `flatcc_builder_end_buffer` (creates a
  real FlatBuffer containing a Vec3 struct)
- `flatcc_builder_get_direct_buffer` (reads serialised bytes back)

## Build

From a Zephyr workspace:

```sh
west build -p always -s <path-to>/flatcc-zephyr/samples/flatcc_basic -b native_sim
west build -t run
```

## Expected output

```text
*** Booting Zephyr OS build v4.4.0-rc2 ***
builder initialised (allocator OK)
created 16-byte FlatBuffer with Vec3 struct
PASS
builder cleared — all memory freed
```

## Configuration

See [`prj.conf`](prj.conf). Notable settings:

- `CONFIG_HEAP_MEM_POOL_SIZE=4096` — required for `k_malloc` to work

Upstream FlatCC must exist at `modules/lib/flatcc`.
