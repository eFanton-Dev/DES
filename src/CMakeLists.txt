add_executable(DES main.cpp
    parser.cpp
    options.cpp
    execute.cpp
)

target_link_libraries(DES 
    PRIVATE 
        utils
        DESLib        
)