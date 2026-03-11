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

add_library(AddCubePlugin SHARED
    AddCubePluginEntry.cpp
    AddCubeCallback.cpp
    CubeVertexGenCallable.cpp
    AddCubeCommand.cpp
)

target_link_libraries(AddCubePlugin PRIVATE
    SurfaceEditorCore
)

target_include_directories(AddCubePlugin PRIVATE
    ${CMAKE_SOURCE_DIR}
)

# Získanie a vypísanie adresárov, ktoré sú nastavené pre Include Directories pre AddCubePlugin
get_target_property(PLUGIN_INCLUDE_DIRS AddCubePlugin INCLUDE_DIRECTORIES)

# Vytlačenie cesty k Include adresárom
message(STATUS "AddCubePlugin include directories: ${PLUGIN_INCLUDE_DIRS}")
