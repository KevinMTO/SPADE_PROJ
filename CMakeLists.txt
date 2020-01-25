cmake_minimum_required(VERSION 3.10)
project(SPADE_PROJ C)

set(CMAKE_C_STANDARD 99)

add_executable(SPADE_PROJ main.c processing.c util.c pseudo_pruning.c)