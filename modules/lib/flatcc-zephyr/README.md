# flatcc-zephyr

Zephyr overlay module that integrates upstream
[dvidelabs/flatcc](https://github.com/dvidelabs/flatcc) runtime sources
without modifying upstream files.

This module builds only the FlatCC runtime library (`flatccrt`) for target use.
It does not build the host `flatcc` compiler, tests, or samples.

Note: repository homepage documentation is in the root `README.md`.
This module README stays next to module code for module-local integration details.

## Expected layout

Inside the flatcc-zephyr repository, upstream FlatCC (a pinned git
submodule) lives at `modules/lib/flatcc`, as a sibling of this overlay at
`modules/lib/flatcc-zephyr`. The CMake here resolves upstream relative to
that sibling relationship.

## Enable in a Zephyr app

In a **west workspace** with flatcc-zephyr in the manifest (with
`submodules: true`), nothing is needed: the repo-root `zephyr/module.yml`
makes Zephyr discover the module automatically.

For a **standalone checkout**, register the flatcc-zephyr repo root with
`EXTRA_ZEPHYR_MODULES` before `find_package(Zephyr ...)`:

```cmake
list(APPEND EXTRA_ZEPHYR_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/path/to/flatcc-zephyr
)
```

(Pointing at this nested overlay directory instead of the repo root also
works, but never register both — Zephyr rejects duplicate module names.)

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

Notes:

- The Zephyr backend requires a kernel heap: set
  `CONFIG_HEAP_MEM_POOL_SIZE` large enough for your buffers (enforced by a
  compile-time assert).
- The allocation macros are overridden **for the `flatccrt` library
  translation units only**. Application code that invokes FlatCC's
  allocation macros directly (e.g. `FLATCC_ALLOC`, the aligned-alloc
  helpers inlined from `flatcc_alloc.h`) gets the libc defaults. All normal
  builder/emitter API usage allocates inside the library, so this only
  matters if you call those macros yourself.

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
