cmake_minimum_required(VERSION 3.16)
project(AddCubePlugin)
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
add_library(AddNodesToOutlinerLayerPlugin SHARED
    AddNodesToOutlinerLayerPluginEntry.cpp
    AddOutlinerNodeCallBacks.cpp
    MeshOutlinerAdderCallable.cpp
)

target_link_libraries(AddNodesToOutlinerLayerPlugin PRIVATE
    SurfaceEditorCore
)

target_include_directories(AddNodesToOutlinerLayerPlugin PRIVATE
    ${CMAKE_SOURCE_DIR}
)
