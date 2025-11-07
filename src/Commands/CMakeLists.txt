message(STATUS ">>> Commands CMake included")

file(GLOB COMMAND_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

if(DEFINED SRC_FILES)
    foreach(file ${COMMAND_FILES})
        list(REMOVE_ITEM SRC_FILES "${file}")
    endforeach()
endif()

foreach(file ${COMMAND_FILES})
    get_filename_component(cmdName ${file} NAME_WE)
    string(TOUPPER ${cmdName} CMDNAME_UPPER)
    set(optName "COMMAND_ENABLE_${CMDNAME_UPPER}")
    option(${optName} "Enable ${cmdName}" ON)
endforeach()

foreach(file ${COMMAND_FILES})
    get_filename_component(cmdName ${file} NAME_WE)
    string(TOUPPER ${cmdName} CMDNAME_UPPER)
    set(optName "COMMAND_ENABLE_${CMDNAME_UPPER}")

    if(${optName})
        message(STATUS "Including command: ${cmdName}")
        list(APPEND SRC_FILES "${file}")
    else()
        message(STATUS "Skipping command: ${cmdName}")
    endif()
endforeach()
