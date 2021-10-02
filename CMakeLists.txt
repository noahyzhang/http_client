cmake_minimum_required(VERSION 3.17)
project(http_client)

set(CMAKE_CXX_STANDARD 11)

include_directories(include/)
include_directories(source/)

aux_source_directory(source LIB_DIR_SOURCE)

add_library(http_client STATIC
        ${LIB_DIR_SOURCE})