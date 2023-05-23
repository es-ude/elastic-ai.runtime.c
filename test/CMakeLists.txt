################## test_broker ##################
add_executable(test_broker
        exampleLocalBroker/test_broker.c)
target_link_libraries(test_broker
        unity
        exampleLocalBroker
        protocol)
add_test(NAME test_broker COMMAND test_broker)

################## test_protocol ##################
add_executable(test_protocol
        protocol/test_protocol.c)
target_link_libraries(test_protocol
        unity
        protocol
        exampleLocalBroker
        topicMatcher)
add_test(NAME test_protocol COMMAND test_protocol)
