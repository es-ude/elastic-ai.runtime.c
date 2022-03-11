cmake_minimum_required(VERSION 3.21)
project("elastic-AI.runtime.c" VERSION 0.0.1 LANGUAGES C)

# Let's tell it a bit about our environment
# Note: With this line, CTest does not find a compiled test
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# We declare a variable to select what to build set
SET(TARGET_GROUP production CACHE STRING "What Should I Build?")

# Modules that exist in both our release and our unit tests, are added now
add_subdirectory(src)

if (TARGET_GROUP STREQUAL production)
    # If we had any files that ONLY were compiled into our release, for example startup code,
    # it's really handy to put it here.

elseif (TARGET_GROUP STREQUAL test)
    # Our tests need to let CTest know we're running tests
    include(CTest)

    # Include Unity Dependency
    add_subdirectory(extern/unity)

    # It also needs to understand we have code in source and test directories
    add_subdirectory(test)

    # It should give verbose test output when a test fails
    list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

else ()
    message(FATAL_ERROR "I don't know the TARGET_GROUP you gave me!")
endif ()

# For external use
add_library(elastic-ai.runtime.c
        ${CMAKE_CURRENT_LIST_DIR}/src/posting.c
        ${CMAKE_CURRENT_LIST_DIR}/src/posting.h
        ${CMAKE_CURRENT_LIST_DIR}/src/protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/src/protocol.h
        ${CMAKE_CURRENT_LIST_DIR}/src/communicationEndpoint.h
        ${CMAKE_CURRENT_LIST_DIR}/src/subscriber.h)
target_include_directories(elastic-ai.runtime.c PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
