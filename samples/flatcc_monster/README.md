# flatcc Monster sample

Zephyr port of the upstream FlatCC
[Monster tutorial](https://github.com/dvidelabs/flatcc/tree/master/samples/monster).
Demonstrates schema-generated FlatBuffer creation and reading using Zephyr
kernel allocators.

## What it tests

- Schema-generated builder/reader/verifier code (`monster_builder.h`,
  `monster_reader.h`)
- Monster table with all common FlatBuffer types: scalars, strings, structs
  (`Vec3`), vectors (`inventory`, `weapons`), enums (`Color`), and unions
  (`Equipment`)
- `flatcc_builder_finalize_aligned_buffer` and `flatcc_builder_aligned_free`
  with Zephyr `k_malloc`/`k_free`
- Full round-trip: serialize then deserialize and verify every field

## Schema

The Monster schema is in [`monster.fbs`](monster.fbs). Pre-generated headers
are in [`generated/`](generated/).

To regenerate, build the host `flatcc` compiler from the pinned upstream
submodule and run it — the generated code must come from the **same flatcc
version** as the runtime this module compiles (the submodule pin, currently
v0.6.1):

```sh
cd modules/lib/flatcc && scripts/build.sh && cd ../../..
modules/lib/flatcc/bin/flatcc -a -o samples/flatcc_monster/generated \
    samples/flatcc_monster/monster.fbs
```

## Build

From a Zephyr workspace:

```sh
west build -p always -s <path-to>/flatcc-zephyr/samples/flatcc_monster -b native_sim
west build -t run
```

## Expected output

```text
*** Booting Zephyr OS build v4.4.1 ***
Monster FlatBuffer created: 152 bytes
PASS: Monster created and verified successfully
```

## Configuration

See [`prj.conf`](prj.conf). Notable settings:

- `CONFIG_HEAP_MEM_POOL_SIZE=8192` — required for dynamic allocation

The refmap runtime (`CONFIG_FLATCC_RUNTIME_REFMAP`, default `y`) is not
required by this sample; disable it to save code size if unused.
