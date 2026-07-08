add_executable(DES main.cpp)

target_link_libraries(DES 
    PRIVATE 
        utils
        DESLib        
)