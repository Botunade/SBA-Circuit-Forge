# Generate protobuf sources for C++
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../shared_schemas/circuit.proto)

add_library(engine_pulse STATIC PulseEngine.cpp ${PROTO_SRCS})
if(BUILD_FOR_ANDROID)
    target_link_libraries(engine_pulse PRIVATE protobuf::libprotobuf log) # 'log' is for Android logging
else()
    target_link_libraries(engine_pulse PRIVATE protobuf::libprotobuf)
endif()
target_include_directories(engine_pulse PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(engine_pulse PROPERTIES FOLDER "EngineTargets")

# Add the web server as an executable
add_executable(circuit_server server/main.cpp)
target_link_libraries(circuit_server PRIVATE
    engine_pulse
    protobuf::libprotobuf
    Threads::Threads
)
# --- Android Specifics ---
if(BUILD_FOR_ANDROID)
    target_link_libraries(circuit_server PRIVATE log)
    add_definitions(-DASIO_STANDALONE) # Required for standalone Asio
endif()
target_include_directories(circuit_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/server/Crow
    ${asio_SOURCE_DIR}/asio/include
)
set_target_properties(circuit_server PROPERTIES FOLDER "EngineTargets") # Include Crow headers

add_executable(generator generator.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(generator PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(generator PROPERTIES FOLDER "EngineTargets")
target_link_libraries(generator PRIVATE protobuf::libprotobuf)
