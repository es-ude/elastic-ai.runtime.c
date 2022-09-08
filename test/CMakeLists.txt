add_executable(test_broker
        test_broker.c)
target_link_libraries(test_broker
        unity
        broker
        protocol)
add_test(NAME test_broker COMMAND test_broker)

add_executable(test_protocol
        test_protocol.c)
target_link_libraries(test_protocol
        unity
        protocol
        broker
        topicMatcher)
add_test(NAME test_protocol COMMAND test_protocol)
