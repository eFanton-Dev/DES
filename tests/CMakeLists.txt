find_package(Threads REQUIRED)

set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)


add_executable(DES_test 
    utils_test.cpp
    keyschedule_test.cpp
    DES_test.cpp
)

target_compile_definitions(DES_test PRIVATE TESTING)
target_link_libraries(DES_test
  PRIVATE 
    utils
    DESLib
    GTest::gtest_main 
    GTest::gmock
)

target_compile_definitions(DESLib PUBLIC TESTING)

include(GoogleTest)
gtest_discover_tests(DES_test)