# flatcc-zephyr

Zephyr overlay module that integrates upstream
[dvidelabs/flatcc](https://github.com/dvidelabs/flatcc) runtime sources
without modifying upstream files.

This module builds only the FlatCC runtime library (`flatccrt`) for target use.
It does not build the host `flatcc` compiler, tests, or samples.

Note: repository homepage documentation is in the root `README.md`.
This module README stays next to module code for module-local integration details.

## Expected layout

Upstream FlatCC must be present at:

`modules/lib/flatcc`

This overlay must be present at:

`modules/lib/flatcc-zephyr`

## Enable in a Zephyr app

Add this overlay module path with `ZEPHYR_EXTRA_MODULES`, for example:

```cmake
list(APPEND ZEPHYR_EXTRA_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/lib/flatcc-zephyr
)
```

## Kconfig options

- `CONFIG_FLATCC`: enable FlatCC runtime integration.
- `CONFIG_FLATCC_ALLOC_ZEPHYR`: use Zephyr allocators (default).
- `CONFIG_FLATCC_ALLOC_LIBC`: use libc allocators.
- `CONFIG_FLATCC_RUNTIME_REFMAP`: build `refmap.c` (default `y`).
- `CONFIG_FLATCC_RUNTIME_JSON`: build JSON runtime (`json_parser.c` and
  `json_printer.c`), default `n` to reduce code size.

## Allocation behavior

Default allocation mode maps FlatCC allocation macros to Zephyr APIs:

- `FLATCC_ALLOC` -> `k_malloc`
- `FLATCC_FREE` -> `k_free`
- `FLATCC_CALLOC` -> `flatcc_zephyr_calloc` -> `k_calloc`
- `FLATCC_REALLOC` -> `flatcc_zephyr_realloc` -> `k_realloc`

Set `CONFIG_FLATCC_ALLOC_LIBC=y` to switch to libc allocation macros.

## Linking

The module creates a Zephyr library target named `flatccrt`.
Link it from your application:

```cmake
target_link_libraries(app PRIVATE flatccrt)
```

## Samples

| Sample | Description |
|--------|-------------|
| `samples/flatcc_basic` | Raw builder API test — no schema-generated code |
| `samples/flatcc_monster` | Full Monster tutorial — schema-generated code |
