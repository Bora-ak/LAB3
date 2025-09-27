cmake_minimum_required(VERSION 3.16)
project(lab3_socket_app)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include(GNUInstallDirs)

# Generate protobuf sources into the binary dir
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/simple_message.proto)

# Library that includes UDP code + generated protobuf sources
add_library(lab3net STATIC src/udp.cpp ${PROTO_SRCS})
target_include_directories(lab3net PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}          # for generated .pb.h
)
target_link_libraries(lab3net PUBLIC ${Protobuf_LIBRARIES})

# Server / Client
add_executable(start_server apps/server.cpp)
target_link_libraries(start_server PRIVATE lab3net)

add_executable(start_client apps/client.cpp)
target_link_libraries(start_client PRIVATE lab3net)

install(TARGETS start_server start_client
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
