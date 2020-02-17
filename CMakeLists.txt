cmake_minimum_required(VERSION 3.15)
project(Maman14 C)

set(CMAKE_C_STANDARD 90)

add_executable(Maman14 main.c command.c command.h definitions.h definitions.c symbol.c symbol.h io_maneger.c io_maneger.h object.c object.h)

target_link_libraries(Maman14 m)