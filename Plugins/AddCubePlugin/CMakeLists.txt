cmake_minimum_required(VERSION 3.16)
project(AddCubePlugin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(AddCubePlugin SHARED
    AddCubePluginEntry.cpp
    AddCubeCallback.cpp
    CubeVertexGenCallable.cpp
    AddCubeCommand.cpp
)

target_include_directories(AddCubePlugin PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(AddCubePlugin PRIVATE
    SurfaceEditorCore
)

set_target_properties(AddCubePlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)