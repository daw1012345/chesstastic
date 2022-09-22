add_executable(chesstastic
        src/main.cpp)

target_link_libraries(chesstastic chesstastic_lib
        )

target_include_directories(chesstastic
        PRIVATE include/chesstastic
        INTERFACE include/
        )