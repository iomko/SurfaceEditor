cmake_minimum_required(VERSION 3.16)
project(SurfaceEditor VERSION 1.0 LANGUAGES C CXX)

add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/src/Renderer/Shaders")
add_compile_definitions(ML_DATA_DIR="${CMAKE_SOURCE_DIR}/src/Ml/ModelsData")

#use C++20 features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(ExternalProject)

set(LIBTORCH_URL
  "https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-2.4.0%2Bcpu.zip"
  CACHE STRING "URL to libtorch zip"
)

set(LIBTORCH_PREFIX "${CMAKE_BINARY_DIR}/_deps/libtorch_ep")
set(LIBTORCH_SRC    "${LIBTORCH_PREFIX}/src")
set(LIBTORCH_ROOT   "${LIBTORCH_SRC}")
set(LIBTORCH_LIBDIR "${LIBTORCH_ROOT}/lib")

file(MAKE_DIRECTORY "${LIBTORCH_SRC}/include")
file(MAKE_DIRECTORY "${LIBTORCH_SRC}/lib")

ExternalProject_Add(libtorch_ep
  URL "${LIBTORCH_URL}"
  PREFIX "${LIBTORCH_PREFIX}"
  DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/_deps/downloads"
  SOURCE_DIR "${LIBTORCH_SRC}"

  STAMP_DIR "${LIBTORCH_PREFIX}/stamp"
  TMP_DIR   "${LIBTORCH_PREFIX}/tmp"

  BINARY_DIR "${LIBTORCH_PREFIX}/build"

  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

#FIND
find_package(X11 REQUIRED)

find_package(glfw3 3.3 REQUIRED)

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
    GIT_TAG v1.92.5-docking                                  
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(ImGui
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(ImGui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
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

FetchContent_Declare(
    ImGuizmo
    GIT_REPOSITORY https://github.com/Gl1tchs/ImGuizmo.git
    GIT_TAG master
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(ImGuizmo)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL=1)


file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(SurfaceEditor ${SRC_FILES})

add_dependencies(SurfaceEditor libtorch_ep)

#find libtorch .so
set_property(TARGET SurfaceEditor PROPERTY BUILD_RPATH "${LIBTORCH_LIBDIR}")
set_property(TARGET SurfaceEditor PROPERTY INSTALL_RPATH "${LIBTORCH_LIBDIR}")

add_library(Torch::Torch INTERFACE IMPORTED GLOBAL)
set_property(TARGET Torch::Torch PROPERTY
  INTERFACE_LINK_LIBRARIES
  "${LIBTORCH_LIBDIR}/libtorch.so;${LIBTORCH_LIBDIR}/libtorch_cpu.so;${LIBTORCH_LIBDIR}/libc10.so"
)

target_include_directories(Torch::Torch INTERFACE
  "${LIBTORCH_ROOT}/include"
)

target_include_directories(SurfaceEditor PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
  ${ImGuizmo_SOURCE_DIR}
)

target_link_libraries(SurfaceEditor PRIVATE
  glad_core glfw OpenGL::GL glm::glm ImGui ImGuizmo
  CURL::libcurl Eigen3::Eigen TIFF::TIFF TrianglePP X11::X11
  Torch::Torch
)
