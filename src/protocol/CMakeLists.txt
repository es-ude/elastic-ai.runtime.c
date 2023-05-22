################## PROTOCOL LIBRARY ##################
add_library(protocol STATIC
        Protocol.c
        Protocol.h
        ProtocolInternal.h
        Posting.h
        Status.h
        CommunicationEndpoint.h)
target_include_directories(protocol PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})
