cmake_minimum_required(VERSION 3.16)
project(FetchSurfacePlugin)

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

if(NOT TARGET CURL::libcurl)
    find_package(CURL REQUIRED)
endif()

if(NOT TARGET TIFF::TIFF)
    find_package(TIFF REQUIRED)
endif()

add_library(FetchSurfacePlugin SHARED
    FetchSurfacePluginEntry.cpp
    FetchSurfaceCallBack.cpp
    FetchSurfaceCommand.cpp
    FetchedSurfaceVertexGenCallable.cpp
)

target_link_libraries(FetchSurfacePlugin PRIVATE
    SurfaceEditorCore
    CURL::libcurl
    TIFF::TIFF
)

target_include_directories(FetchSurfacePlugin PRIVATE
    ${CMAKE_SOURCE_DIR}
)