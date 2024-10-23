cmake_minimum_required(VERSION 3.29)
project(linux_api C)

set(CMAKE_C_STANDARD 99)

add_executable(us_xfr_sv lib/error_functions.c sockets/us_xfr_sv.c)
add_executable(us_xfr_cl lib/error_functions.c sockets/us_xfr_cl.c)
add_executable(ud_ucase_sv lib/error_functions.c sockets/ud_ucase_sv.c)
add_executable(ud_ucase_cl lib/error_functions.c sockets/ud_ucase_cl.c)
add_executable(us_abstract_bind lib/error_functions.c sockets/us_abstract_bind.c)