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

# Add subdirectories for our C++ pillars
add_subdirectory(engine_pulse)
add_subdirectory(ui_canvas)
