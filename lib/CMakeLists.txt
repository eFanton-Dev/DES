add_library(utils STATIC utils.cpp)

target_include_directories(utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (MSVC)
    target_compile_options(utils PRIVATE /W4)
else()
    target_compile_options(utils PRIVATE -Wall -Wextra)
endif()


add_library(DESLib 
  STATIC 
    DES.cpp
    KeySchedule.cpp
)

target_include_directories(DESLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(DESLib
  PUBLIC
    utils
)

if (MSVC)
    target_compile_options(DESLib PRIVATE /W4)
else()
    target_compile_options(DESLib PRIVATE -Wall -Wextra)
endif()