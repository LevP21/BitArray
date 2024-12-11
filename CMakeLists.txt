cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(BitArray VERSION 0.1 LANGUAGES CXX)

option(ENABLE_TESTS "Enable or disable tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)

target_include_directories(bitarray_lib PUBLIC .) # PRIVATE

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()