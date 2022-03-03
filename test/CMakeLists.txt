add_executable(test_protocol test_protocol.c)
target_link_libraries(test_protocol unity)
add_test(NAME test_protocol COMMAND test_protocol)

add_executable(test_posting test_posting.c)
target_link_libraries(test_posting unity posting)
add_test(NAME test_posting COMMAND test_posting)

add_executable(test_publishSubscribe test_publishSubscribe.c)
target_link_libraries(test_publishSubscribe unity broker protocol posting)
add_test(NAME test_publishSubscribe COMMAND test_publishSubscribe)
