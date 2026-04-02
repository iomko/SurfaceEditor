cmake_minimum_required(VERSION 3.16)
project(SurfaceEditor VERSION 1.0 LANGUAGES C CXX)

add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/src/Renderer/Shaders")
add_compile_definitions(ML_DATA_DIR="${CMAKE_SOURCE_DIR}/src/Ml/ModelsData")

#use C++20 features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(ExternalProject)

if(UNIX)
    set(LIBTORCH_URL
    "https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-2.4.0%2Bcpu.zip"
    CACHE STRING "URL to libtorch zip"
    )
else()
    set(LIBTORCH_URL
    "https://download.pytorch.org/libtorch/cpu/libtorch-win-shared-with-deps-2.4.0%2Bcpu.zip"
    CACHE STRING "URL to libtorch zip"
    )
endif()

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

# Platform-specific dependencies
if(UNIX)
    find_package(X11 REQUIRED)
endif()

find_package(glfw3 CONFIG REQUIRED)

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
target_link_libraries(ImGui PUBLIC glfw)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        73db193f853e2ee079bf3ca8a64aa2eaf6459043
    GIT_PROGRESS   TRUE
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad)
glad_add_library(glad_core STATIC REPRODUCIBLE LOADER API gl:core=3.3)
set_target_properties(glad_core PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

FetchContent_Declare(
    ImGuizmo
    GIT_REPOSITORY https://github.com/Gl1tchs/ImGuizmo.git
    GIT_TAG master
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(ImGuizmo)

FetchContent_Declare(
    ImGuiFileDialog
    GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog.git
    GIT_TAG master
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(ImGuiFileDialog)

target_include_directories(ImGuiFileDialog PUBLIC
    ${imguifiledialog_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
)
target_link_libraries(ImGuiFileDialog PUBLIC ImGui)


add_compile_definitions(GLM_ENABLE_EXPERIMENTAL=1)

file(GLOB_RECURSE CORE_SRC
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# Teraz vytvor executable
add_library(SurfaceEditorCore SHARED ${CORE_SRC})

add_dependencies(SurfaceEditorCore libtorch_ep)

#find libtorch .so
if(UNIX)
  set_property(TARGET SurfaceEditorCore PROPERTY BUILD_RPATH "${LIBTORCH_LIBDIR}")
  set_property(TARGET SurfaceEditorCore PROPERTY INSTALL_RPATH "${LIBTORCH_LIBDIR}")
endif()


add_library(Torch::Torch INTERFACE IMPORTED GLOBAL)
if(WIN32)
  set_property(TARGET Torch::Torch PROPERTY
    INTERFACE_LINK_LIBRARIES
    "${LIBTORCH_LIBDIR}/torch.lib;${LIBTORCH_LIBDIR}/torch_cpu.lib;${LIBTORCH_LIBDIR}/c10.lib"
  )
else()
  set_property(TARGET Torch::Torch PROPERTY
    INTERFACE_LINK_LIBRARIES
    "${LIBTORCH_LIBDIR}/libtorch.so;${LIBTORCH_LIBDIR}/libtorch_cpu.so;${LIBTORCH_LIBDIR}/libc10.so"
  )
endif()

target_include_directories(Torch::Torch INTERFACE
  "${LIBTORCH_ROOT}/include"
)
target_link_libraries(SurfaceEditorCore PUBLIC glad_core ImGui ImGuiFileDialog ImGuizmo glfw Torch::Torch)
target_link_libraries(SurfaceEditorCore PRIVATE OpenGL::GL glm::glm CURL::libcurl Eigen3::Eigen TIFF::TIFF TrianglePP)

if(UNIX)
    target_link_libraries(SurfaceEditorCore PRIVATE X11::X11)
endif()

target_include_directories(SurfaceEditorCore PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${ImGuizmo_SOURCE_DIR}
    ${imgui_SOURCE_DIR}  
    ${glad_SOURCE_DIR}
)
set_target_properties(SurfaceEditorCore PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
add_executable(SurfaceEditor
    "${PROJECT_SOURCE_DIR}/src/Core/main.cpp"
)

target_link_libraries(SurfaceEditor PRIVATE
    SurfaceEditorCore
)

get_target_property(CORE_INCLUDE_DIRS SurfaceEditorCore INCLUDE_DIRECTORIES)

set_target_properties(ImGuizmo PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/_deps/imguizmo-src;${CMAKE_BINARY_DIR}/_deps/imgui-src"  # Používame absolútne cesty
  INTERFACE_LINK_LIBRARIES "ImGui;ImGuizmo"  # Linkovanie knižníc ImGui a ImGuizmo
)

target_compile_definitions(SurfaceEditorCore PUBLIC
    SHADER_DIR="${CMAKE_SOURCE_DIR}/src/Renderer/Shaders"
    ML_DATA_DIR="${CMAKE_SOURCE_DIR}/src/Ml/ModelsData"
)


function(add_surface_editor_plugin PLUGIN_NAME)

    add_subdirectory(Plugins/${PLUGIN_NAME})

    set_target_properties(${PLUGIN_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    )

endfunction()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Plugins)
file(GLOB PLUGINS Plugins/*)

foreach(plugin ${PLUGINS})
    message(STATUS "Looking for plugin in: ${plugin}")
    get_filename_component(plugin_name ${plugin} NAME)

    option(BUILD_PLUGIN_${plugin_name} "Build plugin ${plugin_name}" ON)

    if(BUILD_PLUGIN_${plugin_name})
        add_surface_editor_plugin(${plugin_name})
    endif()
endforeach()

export(TARGETS SurfaceEditorCore glad_core ImGui ImGuizmo ImGuiFileDialog TrianglePP Torch::Torch FILE SurfaceEditorCoreConfig.cmake)
