cmake_minimum_required(VERSION 3.29)
project(linux_api C)

set(CMAKE_C_STANDARD 99)

add_executable(linux_api
        main.c
        lib/error_functions.c
        lib/tlpi_hdr.h
        lib/get_num.c
)
