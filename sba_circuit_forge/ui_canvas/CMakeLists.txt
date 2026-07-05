# ui_canvas CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# Find Qt again just in case, though it's in parent
find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)
find_package(Protobuf REQUIRED)

# Generate C++ sources for circuit.proto for UI canvas too
set(PROTO_FILE ${CMAKE_SOURCE_DIR}/shared_schemas/circuit.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})

add_executable(ui_canvas
    main.cpp
    MainWindow.cpp
    MainWindow.h
    SbaCircuitModule.cpp
    SbaCircuitModule.h
    NetworkManager.cpp
    NetworkManager.h
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

# Set include directories
target_include_directories(ui_canvas PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} # For generated protobuf headers
)

target_link_libraries(ui_canvas PRIVATE
    Qt5::Widgets
    Qt5::Network
    ${Protobuf_LIBRARIES}
)