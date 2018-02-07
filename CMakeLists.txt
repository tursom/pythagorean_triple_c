cmake_minimum_required(VERSION 3.9)
project(pythagorean_triple_c C)

set(CMAKE_C_STANDARD 99)

add_executable(pythagorean_triple_c main.c)
target_link_libraries(pythagorean_triple_c m)