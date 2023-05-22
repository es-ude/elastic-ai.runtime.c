################## PROTOCOL LIBRARY ##################
add_library(exampleLocalBroker STATIC
        ExampleLocalBroker.c
        ExampleLocalBroker.h)
target_link_libraries(exampleLocalBroker PUBLIC
        topicMatcher
        protocol)
target_include_directories(exampleLocalBroker PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})
