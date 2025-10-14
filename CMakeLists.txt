cmake_minimum_required(VERSION 3.16)
project(SurfaceEditor VERSION 1.0 LANGUAGES C CXX)

add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/src/Renderer/Shaders")

#use C++20 features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#FIND
find_package(X11 REQUIRED)

find_package(glfw3 3.4 REQUIRED)

find_package(OpenGL REQUIRED)

find_package(glm REQUIRED)

find_package(TIFF REQUIRED)

find_package(CURL REQUIRED)

find_package(Eigen3 REQUIRED)

#FETCH
include(FetchContent)

FetchContent_Declare(
    TrianglePP
    GIT_REPOSITORY https://github.com/mrkkrj/TrianglePP.git
    GIT_TAG master
)
FetchContent_MakeAvailable(TrianglePP)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git  
    GIT_TAG docking                                      
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui_lib
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        73db193f853e2ee079bf3ca8a64aa2eaf6459043
    GIT_PROGRESS   TRUE
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad)
glad_add_library(glad_core STATIC REPRODUCIBLE LOADER API gl:core=3.3)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(SurfaceEditor ${SRC_FILES})

target_link_libraries(SurfaceEditor PRIVATE glad_core glfw3 OpenGL::GL glm::glm imgui_lib CURL::libcurl Eigen3::Eigen TIFF::TIFF TrianglePP X11::X11)

target_include_directories(SurfaceEditor PRIVATE "${PROJECT_SOURCE_DIR}/src")

target_include_directories(imgui_lib PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
