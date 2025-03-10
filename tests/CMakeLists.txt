cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(bitarray_tests VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(googletest
URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(bitarray_tests bitarray_tests.cpp)

target_link_libraries(bitarray_tests PRIVATE GTest::gtest_main bitarray_lib)

include(GoogleTest)
gtest_discover_tests(bitarray_tests)