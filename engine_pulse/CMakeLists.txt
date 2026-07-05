# Generate protobuf sources for C++
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../shared_schemas/circuit.proto)

add_executable(engine_pulse main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(engine_pulse PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(engine_pulse PRIVATE protobuf::libprotobuf)
