cmake_minimum_required(VERSION 3.21)
project("elastic-AI.runtime.c" VERSION 0.0.1 LANGUAGES C)

get_directory_property(hasParent PARENT_DIRECTORY)

add_subdirectory(src)

if (hasParent)
    include(src/elastic-ai.runtime.cmake)
    add_library(elastic-ai.runtime.c ${ELASTIC-AI-RUNTIME-LIB})
    target_link_libraries(elastic-ai.runtime.c
            topicMatcher
            protocol)
else ()
    # Include submodules
    include(helpers.cmake)
    fetch_git_submodules()

    include(CTest)
    add_subdirectory(extern/unity)
    add_subdirectory(test)
    list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
endif ()
