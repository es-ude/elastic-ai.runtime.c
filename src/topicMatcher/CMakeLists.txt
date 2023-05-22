################## PROTOCOL LIBRARY ##################
add_library(topicMatcher STATIC
        topicMatcher.c
        topicMatcher.h
        topicMatcherInternal.h)
target_include_directories(topicMatcher PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})
