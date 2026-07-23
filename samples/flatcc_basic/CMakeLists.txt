cmake_minimum_required(VERSION 3.20.0)

# Repo root carries zephyr/module.yml; registering it here covers
# standalone checkouts, and west workspaces discover the same path anyway.
list(APPEND EXTRA_ZEPHYR_MODULES
  ${CMAKE_CURRENT_LIST_DIR}/../..
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(flatcc_basic)

target_sources(app PRIVATE src/main.c)
target_link_libraries(app PRIVATE flatccrt)
