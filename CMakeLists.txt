cmake_minimum_required(VERSION 3.29)
project(linux_api C)

set(CMAKE_C_STANDARD 99)

add_executable(us_xfr_sv lib/error_functions.c sockets/us_xfr_sv.c)
add_executable(us_xfr_cl lib/error_functions.c sockets/us_xfr_cl.c)