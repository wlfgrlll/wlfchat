cmake_minimum_required(VERSION 3.28)
project(wlfchat C)

set(CMAKE_C_STANDARD 11)

add_executable(wlfchat main.c
        wlfchatutils.c
        wlfchatutils.h)
find_library(TERMBOX2 termbox2)
target_link_libraries(wlfchat LINK_PUBLIC ${TERMBOX2})