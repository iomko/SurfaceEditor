cmake_minimum_required(VERSION 3.16)
project(BrushToolPlugin)
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
add_library(BrushToolPlugin SHARED
    BrushToolPluginEntry.cpp
    BrushTool.cpp
    BrushToolCallBack.cpp
    BrushToolCommand.cpp
)

target_link_libraries(BrushToolPlugin PRIVATE
    SurfaceEditorCore
)

target_include_directories(BrushToolPlugin PRIVATE
    ${CMAKE_SOURCE_DIR}
)

