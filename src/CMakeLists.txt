add_executable(DES
    main.cpp
    parser.cpp
)

target_link_libraries(DES 
    PRIVATE 
        utils
        DESLib
        App
)