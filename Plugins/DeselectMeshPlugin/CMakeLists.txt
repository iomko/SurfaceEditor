cmake_minimum_required(VERSION 3.16)
project(DeselectMeshPlugin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL=1)

find_package(SurfaceEditorCore REQUIRED PATHS "${CMAKE_SOURCE_DIR}/../../build" NO_DEFAULT_PATH)

add_library(DeselectMeshPlugin SHARED
    DeselectMeshPluginEntry.cpp
    DeselectMeshCallBack.cpp
    DeselectMeshCommand.cpp
    MeshDeselectionTool.cpp
)

target_link_libraries(DeselectMeshPlugin PRIVATE
    SurfaceEditorCore
)

target_include_directories(DeselectMeshPlugin PRIVATE
    ${CMAKE_SOURCE_DIR}
)

