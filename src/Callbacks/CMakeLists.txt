message(STATUS ">>> Callbacks CMake included")

file(GLOB CALLBACK_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

if(DEFINED SRC_FILES)
    foreach(file ${CALLBACK_FILES})
        list(REMOVE_ITEM SRC_FILES "${file}")
    endforeach()
endif()

foreach(file ${CALLBACK_FILES})
    get_filename_component(cmdName ${file} NAME_WE)
    string(TOUPPER ${cmdName} CMDNAME_UPPER)
    set(optName "CALLBACK_ENABLE_${CMDNAME_UPPER}")
    option(${optName} "Enable ${cmdName}" ON)
endforeach()

foreach(file ${CALLBACK_FILES})
    get_filename_component(cmdName ${file} NAME_WE)
    string(TOUPPER ${cmdName} CMDNAME_UPPER)
    set(optName "CALLBACK_ENABLE_${CMDNAME_UPPER}")

    if(${optName})
        message(STATUS "Including callback: ${cmdName}")
        list(APPEND SRC_FILES "${file}")
    else()
        message(STATUS "Skipping callback: ${cmdName}")
    endif()
endforeach()
