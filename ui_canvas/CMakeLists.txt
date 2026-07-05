# Generate protobuf sources for C++
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../shared_schemas/circuit.proto)

add_executable(ui_canvas main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(ui_canvas PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(ui_canvas PRIVATE protobuf::libprotobuf)
