# flatcc-zephyr

[![docs](https://img.shields.io/github/actions/workflow/status/beriberikix/flatcc-zephyr/docs.yml?branch=main&label=docs)](https://beriberikix.github.io/flatcc-zephyr/)
[![tests](https://img.shields.io/github/actions/workflow/status/beriberikix/flatcc-zephyr/test.yml?branch=main&label=tests)](https://github.com/beriberikix/flatcc-zephyr/actions/workflows/test.yml)

Zephyr-native overlay module for upstream
[FlatCC](https://github.com/dvidelabs/flatcc) runtime integration.

This repository provides:

- A Zephyr overlay module that builds FlatCC runtime sources from upstream
  `dvidelabs/flatcc` without patching upstream files.
- Zephyr-first allocator mapping (default): `k_malloc`, `k_free`, `k_calloc`,
  `k_realloc`.
- Sample apps that validate module integration and demonstrate FlatBuffer
  usage on Zephyr.

## Repository layout

```
zephyr/module.yml            Repo-root module wrapper (west auto-discovery)
modules/lib/flatcc-zephyr/   Zephyr module overlay (Kconfig, CMake, allocator shim)
modules/lib/flatcc/          Upstream FlatCC (pinned git submodule)
samples/flatcc_basic/        Minimal raw-builder test (no schema)
samples/flatcc_monster/      Schema-generated Monster example
```

Upstream FlatCC is a **pinned git submodule**, so builds are reproducible.
The pin currently tracks flatcc `main` at `2920173` (April 2026): upstream
has not tagged a release since v0.6.1 (2022), and the intervening
`[0.6.2-pre]` changelog carries relevant fixes (e.g. aligned-alloc failure
handling, verifier hardening). If upstream tags 0.6.2, move the pin there.

## Using from a west workspace (recommended)

Add the module to your manifest; `submodules: true` pulls the upstream
FlatCC sources automatically:

```yaml
manifest:
  projects:
    - name: flatcc-zephyr
      url: https://github.com/beriberikix/flatcc-zephyr
      revision: <pinned-sha-or-tag>
      path: modules/lib/flatcc-zephyr
      submodules: true
```

Zephyr discovers the module automatically through the repo-root
`zephyr/module.yml` — no `EXTRA_ZEPHYR_MODULES` wiring is needed. Enable it
with `CONFIG_FLATCC=y` and link `flatccrt` from your application.

## Standalone checkout

```sh
git clone https://github.com/beriberikix/flatcc-zephyr
cd flatcc-zephyr
git submodule update --init
```

Then register the repo root with `EXTRA_ZEPHYR_MODULES` in your app's
CMakeLists (see the module README for details).

## Quick start

From your Zephyr workspace directory:

```sh
# Basic test — raw builder API, no schema
west build -p always -s <path-to>/flatcc-zephyr/samples/flatcc_basic -b native_sim
west build -t run

# Monster test — schema-generated code
west build -p always -s <path-to>/flatcc-zephyr/samples/flatcc_monster -b native_sim
west build -t run
```

## Samples

| Sample | Description |
|--------|-------------|
| [`samples/flatcc_basic`](samples/flatcc_basic/) | Exercises the raw FlatCC builder/emitter API without schema-generated code. Creates a buffer with a struct, verifies size. |
| [`samples/flatcc_monster`](samples/flatcc_monster/) | Full schema-generated example based on the upstream Monster tutorial. Creates, serializes, and verifies a Monster with strings, vectors, structs, and unions. |

## Module configuration

Module-specific Kconfig options and allocation details are documented in:

- [`modules/lib/flatcc-zephyr/README.md`](modules/lib/flatcc-zephyr/README.md)
