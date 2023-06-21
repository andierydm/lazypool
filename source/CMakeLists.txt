project(lazypool)
cmake_minimum_required(VERSION 3.2)

add_library(lazypool STATIC lazypool.c)
add_executable(main main.c)

target_link_libraries(main PRIVATE lazypool)