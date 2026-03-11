cmake_minimum_required(VERSION 3.16)
project(AddPlanePlugin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL=1)

if(NOT TARGET SurfaceEditorCore)
    find_package(SurfaceEditorCore REQUIRED
        PATHS "${CMAKE_SOURCE_DIR}/../../build"
        NO_DEFAULT_PATH
    )
endif()
add_library(AddPlanePlugin SHARED
    AddPlanePluginEntry.cpp
    AddPlaneCallback.cpp
    PlaneVertexGenCallable.cpp
    AddPlaneCommand.cpp
)

target_link_libraries(AddPlanePlugin PRIVATE
    SurfaceEditorCore
)

target_include_directories(AddPlanePlugin PRIVATE
    ${CMAKE_SOURCE_DIR}
)

