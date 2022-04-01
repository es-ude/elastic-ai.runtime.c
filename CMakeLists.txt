cmake_minimum_required(VERSION 3.21)
project("elastic-AI.runtime.c" VERSION 0.0.1 LANGUAGES C)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Include Source Directory for every build target
add_subdirectory(src)

if (TARGET_GROUP STREQUAL test)
    # Fetch Unity Submodule Submodules
    find_package(Git QUIET)
    if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        # Update Submodules
        option(GIT_SUBMODULE "Check submodules during testing build" ON)
        if(GIT_SUBMODULE)
            message(STATUS "Submodule update")
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            RESULT_VARIABLE GIT_SUBMOD_RESULT)
            if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}")
            endif()
        endif()
    endif()

    # Check Submodules
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/unity/CMakeLists.txt")
        message(FATAL_ERROR "The Unity Submodule was not downloaded! GIT_SUBMODULE was turned off or failed.")
    endif()

    # Our tests need to let CTest know we're running tests
    include(CTest)
    # Include Unity Dependency
    add_subdirectory(extern/unity)
    # It also needs to understand we have code in test directories
    add_subdirectory(test)
	add_subdirectory(examples)
    # It should give verbose test output when a test fails
    list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

elseif (TARGET_GROUP STREQUAL extern)
    # For external use
    add_library(elastic-ai.runtime.c
            ${CMAKE_CURRENT_LIST_DIR}/src/posting.c
            ${CMAKE_CURRENT_LIST_DIR}/src/posting.h
            ${CMAKE_CURRENT_LIST_DIR}/src/protocol.c
            ${CMAKE_CURRENT_LIST_DIR}/src/protocol.h
            ${CMAKE_CURRENT_LIST_DIR}/src/communicationEndpoint.h
            ${CMAKE_CURRENT_LIST_DIR}/src/subscriber.h)
    target_include_directories(elastic-ai.runtime.c PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

else ()
    message(FATAL_ERROR "I don't know the TARGET_GROUP you gave me!")
endif ()
