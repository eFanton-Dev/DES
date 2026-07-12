cmake_minimum_required(VERSION 3.23)

project(DataEncryptionStandard VERSION 1.0 LANGUAGES CXX)

enable_testing() 

add_subdirectory(lib)
add_subdirectory(src)

option(BUILD_TESTS "Build the test suite" ON)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()