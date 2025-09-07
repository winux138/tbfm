cmake_minimum_required(VERSION 3.30)

project(TBFM VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

set(SOURCES
    src/main.cpp
)

add_executable(tbfm ${SOURCES})

find_package(Curses REQUIRED)

target_include_directories(tbfm PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(tbfm PRIVATE ${CURSES_LIBRARIES})

# target_include_directories(tbfm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
