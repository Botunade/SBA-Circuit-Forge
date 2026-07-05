cmake_minimum_required(VERSION 3.16)
project(SBACircuitForge VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt
find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

# Find Protobuf
find_package(Protobuf REQUIRED)

# Add subdirectories
add_subdirectory(engine_pulse)
add_subdirectory(ui_canvas)
