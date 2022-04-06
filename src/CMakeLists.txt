add_library(posting
        posting.c
        posting.h
)
target_include_directories(posting PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(protocol
        protocol.c
        protocol.h
        communicationEndpoint.h
)
target_include_directories(protocol PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(broker
        Broker.c
        Broker.h
        communicationEndpoint.h
)
target_include_directories(broker PUBLIC ${CMAKE_CURRENT_LIST_DIR})
