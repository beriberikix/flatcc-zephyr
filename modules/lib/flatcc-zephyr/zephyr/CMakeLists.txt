# SPDX-License-Identifier: Apache-2.0

if(NOT CONFIG_FLATCC)
  return()
endif()

set(FLATCC_ZEPHYR_OVERLAY_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(FLATCC_UPSTREAM_DIR ${FLATCC_ZEPHYR_OVERLAY_DIR}/../flatcc)
set(FLATCC_RUNTIME_DIR ${FLATCC_UPSTREAM_DIR}/src/runtime)

if(NOT EXISTS ${FLATCC_RUNTIME_DIR}/builder.c)
  message(FATAL_ERROR
    "flatcc-zephyr: expected upstream flatcc runtime at ${FLATCC_RUNTIME_DIR}. "
    "Fetch upstream dvidelabs/flatcc under modules/lib/flatcc.")
endif()

zephyr_library_named(flatccrt)

zephyr_library_include_directories(
  ${FLATCC_UPSTREAM_DIR}/include
  ${FLATCC_ZEPHYR_OVERLAY_DIR}/include
)

# Export includes to consumers of this library
target_include_directories(flatccrt INTERFACE
  ${FLATCC_UPSTREAM_DIR}/include
  ${FLATCC_ZEPHYR_OVERLAY_DIR}/include
)

zephyr_library_sources(
  ${FLATCC_RUNTIME_DIR}/builder.c
  ${FLATCC_RUNTIME_DIR}/emitter.c
  ${FLATCC_RUNTIME_DIR}/verifier.c
)

zephyr_library_sources_ifdef(CONFIG_FLATCC_RUNTIME_REFMAP
  ${FLATCC_RUNTIME_DIR}/refmap.c
)

zephyr_library_sources_ifdef(CONFIG_FLATCC_RUNTIME_JSON
  ${FLATCC_RUNTIME_DIR}/json_parser.c
  ${FLATCC_RUNTIME_DIR}/json_printer.c
)

if(CONFIG_FLATCC_ALLOC_ZEPHYR)
  zephyr_library_sources(
    ${FLATCC_ZEPHYR_OVERLAY_DIR}/src/flatcc_alloc_zephyr.c
  )

  zephyr_library_compile_options(
    -include
    flatcc/flatcc_alloc_zephyr.h
  )

  zephyr_library_compile_definitions(
    FLATCC_ALLOC=k_malloc
    FLATCC_FREE=k_free
    FLATCC_CALLOC=flatcc_zephyr_calloc
    FLATCC_REALLOC=flatcc_zephyr_realloc
  )
else()
  zephyr_library_compile_definitions(
    FLATCC_ALLOC=malloc
    FLATCC_FREE=free
    FLATCC_CALLOC=calloc
    FLATCC_REALLOC=realloc
  )
endif()
