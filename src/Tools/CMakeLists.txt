message(STATUS ">>> Tools CMake included")

file(GLOB TOOL_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

if(DEFINED SRC_FILES)
    foreach(file ${TOOL_FILES})
        list(REMOVE_ITEM SRC_FILES "${file}")
    endforeach()
endif()

foreach(file ${TOOL_FILES})
    get_filename_component(cmdName ${file} NAME_WE)
    string(TOUPPER ${cmdName} CMDNAME_UPPER)
    set(optName "TOOL_ENABLE_${CMDNAME_UPPER}")
    option(${optName} "Enable ${cmdName}" ON)
endforeach()

foreach(file ${TOOL_FILES})
    get_filename_component(cmdName ${file} NAME_WE)
    string(TOUPPER ${cmdName} CMDNAME_UPPER)
    set(optName "TOOL_ENABLE_${CMDNAME_UPPER}")

    if(${optName})
        message(STATUS "Including tool: ${cmdName}")
        list(APPEND SRC_FILES "${file}")
    else()
        message(STATUS "Skipping tool: ${cmdName}")
    endif()
endforeach()
