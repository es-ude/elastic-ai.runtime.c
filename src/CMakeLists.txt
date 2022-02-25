file(GLOB SOURCES ./*.c)

add_library(broker STATIC Broker.c Broker.h)
target_include_directories(broker PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_executable(release ${SOURCES})

target_include_directories(release PUBLIC ${CMAKE_CURRENT_LIST_DIR})
