cmake_minimum_required(VERSION 3.26)
project(intel8080 C)

include_directories(include/)

file(GLOB SOURCES "src/*.c")

set(CMAKE_C_STANDARD 11)

add_executable(intel8080 ${SOURCES})

target_link_libraries(intel8080)
