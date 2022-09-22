cmake_minimum_required(VERSION 3.16)
project(chesstastic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif(CCACHE)

add_subdirectory(lib)
if(NOT WIN32)
    add_subdirectory(app)
endif()
add_subdirectory(tests)
