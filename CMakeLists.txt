cmake_minimum_required(VERSION 3.10)
project(GravitySim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)

# SFML
set(SFML sfml-graphics sfml-window sfml-system)
add_subdirectory(libs/SFML)
include_directories(${SFML_INCLUDE_DIR})

if(APPLE)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

link_libraries(${SFML})

add_executable(app
    src/main.cpp
    src/utils/vec.h
    src/utils/rand.h
    src/particle.h
    src/utils/quadtree.h
    src/utils/quadtree.cpp
)

set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")


if(APPLE OR UNIX)
    target_compile_options(app PRIVATE -Wall -Wextra -Wno-unused-parameter)
elseif(WIN32)
    set_compile_options(app PRIVATE /W4 /WX)
endif()