cmake_minimum_required(VERSION 3.20)
project(net CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(net STATIC
    src/socket.cpp
    src/bufferedreader.cpp
)

target_include_directories(net PUBLIC include)
target_compile_options(net PRIVATE -Wall -Wextra -Wpedantic)
