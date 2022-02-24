add_executable(test_meh_app test_meh.c)
 
target_link_libraries(test_meh_app unity)
 
add_test(NAME test_meh COMMAND test_meh_app)
