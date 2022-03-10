file(GLOB SOURCES ./*.c)

add_library(posting STATIC posting.c posting.h)
target_include_directories(posting PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(protocol STATIC protocol.c protocol.h communicationEndpoint.h)
target_include_directories(protocol PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(broker STATIC Broker.c Broker.h communicationEndpoint.h)
target_include_directories(broker PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_executable(release ${SOURCES})
target_include_directories(release PUBLIC ${CMAKE_CURRENT_LIST_DIR})
