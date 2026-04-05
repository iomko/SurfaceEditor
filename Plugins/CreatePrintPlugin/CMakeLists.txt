cmake_minimum_required(VERSION 3.16)
project(CreatePrintPlugin)
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
add_library(CreatePrintPlugin SHARED
    CreatePrintPluginEntry.cpp
    CreatePrintStructureCallBack.cpp
    CreatePrintCommand.cpp
)

target_link_libraries(CreatePrintPlugin PRIVATE
    SurfaceEditorCore
)

target_include_directories(CreatePrintPlugin PRIVATE
    ${CMAKE_SOURCE_DIR}
)

