cmake_minimum_required(VERSION 3.16)
project(MoveMeshPlugin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL=1)

find_package(SurfaceEditorCore REQUIRED PATHS "${CMAKE_SOURCE_DIR}/../../build" NO_DEFAULT_PATH)

add_library(MoveMeshPlugin SHARED
    MoveMeshPluginEntry.cpp
    MoveMeshCallBack.cpp
    MoveMeshCommand.cpp
)

target_link_libraries(MoveMeshPlugin PRIVATE
    SurfaceEditorCore
)

target_include_directories(MoveMeshPlugin PRIVATE
    ${CMAKE_SOURCE_DIR}
)

