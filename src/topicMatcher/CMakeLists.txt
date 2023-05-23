################## PROTOCOL LIBRARY ##################
add_library(topicMatcher STATIC
        TopicMatcher.c
        TopicMatcher.h
        TopicMatcherInternal.h)
target_include_directories(topicMatcher PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})
