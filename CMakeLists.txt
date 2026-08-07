cmake_minimum_required(VERSION 3.23)

project(DataEncryptionStandard VERSION 4.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

enable_testing() 

add_subdirectory(lib)
add_subdirectory(src)

option(BUILD_TESTS "Build the test suite" OFF)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()