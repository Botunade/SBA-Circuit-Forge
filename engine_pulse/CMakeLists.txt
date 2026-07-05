cmake_minimum_required(VERSION 3.14)
project(SBAPulse)

include(FetchContent)

# Fetch ngspice source repository
FetchContent_Declare(
  ngspice
  GIT_REPOSITORY git://git.code.sf.net/p/ngspice/ngspice
  GIT_TAG master
)

FetchContent_GetProperties(ngspice)
if(NOT ngspice_POPULATED)
  FetchContent_Populate(ngspice)
  # ngspice uses autotools by default, so we'll configure it via ExternalProject_Add instead of add_subdirectory
  include(ExternalProject)
  ExternalProject_Add(
    ngspice_build
    SOURCE_DIR ${ngspice_SOURCE_DIR}
    CONFIGURE_COMMAND ${ngspice_SOURCE_DIR}/autogen.sh && ${ngspice_SOURCE_DIR}/configure --with-ngshared --enable-xspice --disable-debug
    BUILD_COMMAND make -j4
    INSTALL_COMMAND "" # Skip installation phase for local development
    BUILD_IN_SOURCE 1
  )
endif()

# We need the sharedspice.h header
include_directories(${ngspice_SOURCE_DIR}/src/include/ngspice)

# The core executable that wraps ngspice
add_executable(SBA_Pulse SBA_Pulse_Core.cpp)

# Depending on the system, the built library name is libngspice.so (Linux) or libngspice.dylib (macOS)
if(APPLE)
  set(NGSPICE_LIB ${ngspice_SOURCE_DIR}/src/.libs/libngspice.dylib)
elseif(UNIX)
  set(NGSPICE_LIB ${ngspice_SOURCE_DIR}/src/.libs/libngspice.so)
elseif(WIN32)
  set(NGSPICE_LIB ${ngspice_SOURCE_DIR}/src/.libs/libngspice.dll)
endif()

# Add the imported library target so we can link against it
add_library(ngspice_shared SHARED IMPORTED)
set_target_properties(ngspice_shared PROPERTIES IMPORTED_LOCATION ${NGSPICE_LIB})
add_dependencies(ngspice_shared ngspice_build)

target_link_libraries(SBA_Pulse ngspice_shared)
