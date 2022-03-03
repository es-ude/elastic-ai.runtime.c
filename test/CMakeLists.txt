add_executable(test_broker test_broker.c)
target_link_libraries(test_broker unity broker)
add_test(NAME test_broker COMMAND test_broker)

add_executable(test_posting test_posting.c)
target_link_libraries(test_posting unity posting)
add_test(NAME test_posting COMMAND test_posting)

add_executable(test_protocol test_protocol.c)
target_link_libraries(test_protocol unity protocol posting broker)
add_test(NAME test_protocol COMMAND test_protocol)
