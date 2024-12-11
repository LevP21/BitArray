cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(bitarray_lib VERSION 0.1 LANGUAGES CXX)

add_library(bitarray_lib STATIC bitarray.hpp bitarray.cpp)