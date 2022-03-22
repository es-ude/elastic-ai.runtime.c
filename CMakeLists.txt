cmake_minimum_required(VERSION 3.21)
project("elastic-AI.runtime.c" VERSION 0.0.1 LANGUAGES C)

if (TARGET_GROUP STREQUAL test)
    # Include Source directory
    add_subdirectory(src)

    # Our tests need to let CTest know we're running tests
    include(CTest)

    # Include Unity Dependency
    add_subdirectory(extern/unity)

    # It also needs to understand we have code in source and test directories
    add_subdirectory(test)

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
