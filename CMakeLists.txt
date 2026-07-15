cmake_minimum_required(VERSION 3.16)
project(SBACircuitForge VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for dependencies
include(FetchContent)

# If SourceForge times out, tell Jules to use this in the CMakeLists.txt instead:
FetchContent_Declare(
    ngspice
    GIT_REPOSITORY https://github.com/imr/ngspice.git
    GIT_TAG master
)
# Note: ngspice doesn't natively support CMake without custom scripts or Autotools wrapper.
# We will just declare it here as per instructions, though building it might require further setup.
# FetchContent_MakeAvailable(ngspice)

# Find Protobuf
find_package(Protobuf REQUIRED)

# Add a flag to indicate if we are building for Android
option(BUILD_FOR_ANDROID "Build for Android NDK" OFF)

# Fetch Standalone Asio
FetchContent_Declare(
  asio
  URL https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-30-2.tar.gz
)
FetchContent_MakeAvailable(asio)


# Add subdirectories for our C++ pillars
add_subdirectory(engine_pulse)
# The ui_canvas is removed as the UI will be handled by a browser engine.
