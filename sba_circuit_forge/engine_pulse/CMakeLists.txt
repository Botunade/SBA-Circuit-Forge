# engine_pulse CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# Need Protobuf dependency
find_package(Protobuf REQUIRED)

# Generate C++ sources for circuit.proto
set(PROTO_FILE ${CMAKE_SOURCE_DIR}/shared_schemas/circuit.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})

add_library(engine_pulse SHARED
    SBA_Pulse_Core.cpp
    SBA_Pulse_Core.h
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

# Set include directories
target_include_directories(engine_pulse PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} # For generated protobuf headers
)

target_link_libraries(engine_pulse PUBLIC ${Protobuf_LIBRARIES})
