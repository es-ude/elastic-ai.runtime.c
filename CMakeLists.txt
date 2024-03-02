cmake_minimum_required(VERSION 3.20...3.24)
include(helpers.cmake)
project("elastic-AI.runtime.c" LANGUAGES C)

get_directory_property(hasParent PARENT_DIRECTORY)

add_subdirectory(src/topicMatcher)
add_subdirectory(src/protocol)

if (hasParent)
    include(src/elastic-ai.runtime.cmake)
    add_library(elastic-ai.runtime.c ${ELASTIC-AI-RUNTIME-LIB})
    target_link_libraries(elastic-ai.runtime.c
            topicMatcher
            protocol)
else ()
    include(CTest)
    add_unity()
    add_subdirectory(src/exampleLocalBroker)
    add_subdirectory(test)
endif ()
