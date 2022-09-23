################## PROTOCOL LIBRARY ##################
add_library(protocol STATIC
        Protocol.c
        Protocol.h
        ProtocolInternal.h)
target_include_directories(protocol PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

################## TOPIC MATCHER LIBRARY ##################
add_library(topicMatcher STATIC
        TopicMatcher.c
        TopicMatcher.h)
target_include_directories(topicMatcher PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

################## BROKER LIBRARY ##################
add_library(broker STATIC
        ExampleLocalBroker.c
        ExampleLocalBroker.h
        CommunicationEndpoint.h)
target_link_libraries(broker
        topicMatcher)
target_include_directories(broker PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})
