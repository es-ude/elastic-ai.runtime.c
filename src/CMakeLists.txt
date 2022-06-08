add_library(protocol STATIC protocol.c protocol.h)
target_include_directories(protocol PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(topicMatcher STATIC topicMatcher.c topicMatcher.h)
target_include_directories(topicMatcher PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(broker STATIC ExampleLocalBroker.c ExampleLocalBroker.h communicationEndpoint.h)
target_link_libraries(broker
        topicMatcher)
target_include_directories(broker PUBLIC ${CMAKE_CURRENT_LIST_DIR})
