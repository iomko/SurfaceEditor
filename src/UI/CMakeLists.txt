message(STATUS ">>> Layers CMake included")

file(GLOB LAYER_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

if(DEFINED SRC_FILES)
    foreach(file ${LAYER_FILES})
        list(REMOVE_ITEM SRC_FILES "${file}")
    endforeach()
endif()

foreach(file ${LAYER_FILES})
    get_filename_component(cmdName ${file} NAME_WE)
    string(TOUPPER ${cmdName} CMDNAME_UPPER)
    set(optName "LAYER_ENABLE_${CMDNAME_UPPER}")
    option(${optName} "Enable ${cmdName}" ON)
endforeach()

foreach(file ${LAYER_FILES})
    get_filename_component(cmdName ${file} NAME_WE)
    string(TOUPPER ${cmdName} CMDNAME_UPPER)
    set(optName "LAYER_ENABLE_${CMDNAME_UPPER}")

    if(${optName})
        message(STATUS "Including layer: ${cmdName}")
        list(APPEND SRC_FILES "${file}")
    else()
        message(STATUS "Skipping layer: ${cmdName}")
    endif()
endforeach()
