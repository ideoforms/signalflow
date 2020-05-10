cmake_minimum_required(VERSION 3.10.0)
project(Signal C CXX)

#-------------------------------------------------------------------------------
# Use C++11
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MACOSX_RPATH 1)

#-------------------------------------------------------------------------------
# Shared compiler flags.
#-------------------------------------------------------------------------------
add_compile_options(
    -Wall
    -pedantic
    -fPIC
)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_compile_options(
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-vla-extension
    )
endif()

include_directories(
    /usr/local/include
    source/include
    source/lib
    source/lib/pybind11/include
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("Building in debug mode")
    add_compile_options(-ggdb3 -O0 -DDEBUG)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("Building in release mode")
    add_compile_options(-O3 -funroll-loops)
else()
    message("Building in dev mode")
    add_compile_options(-O0)
endif()

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
find_library(SOUNDIO soundio)
if (SOUNDIO)
    message("Found libsoundio")
    add_definitions(-DHAVE_SOUNDIO)
else()
    message(FATAL_ERROR "Couldn't find libsoundio")
endif()

find_library(SNDFILE sndfile)
if (SNDFILE)
    message("Found sndfile")
    add_definitions(-DHAVE_SNDFILE)
else()
    message(FATAL_ERROR "Couldn't find libsndfile")
endif()

find_package(GSL REQUIRED)
message("Found libgsl")
add_definitions(-DHAVE_GSL)

#-------------------------------------------------------------------------------
# For builds of the Python bindings, add Python dependencies and flags
#-------------------------------------------------------------------------------
if (CMAKE_BUILD_PYTHON)
    find_package (PythonLibs 3)
    include_directories(${PYTHON_INCLUDE_DIR})
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
        add_link_options(-w -undefined dynamic_lookup)
    endif()
endif()

#-------------------------------------------------------------------------------
# Specify source files.
# See source/CMakeLists.
#-------------------------------------------------------------------------------
set(SRC)
add_subdirectory("source/src")
set(SRC ${SRC} source/lib/json11/json11.cpp)

#-------------------------------------------------------------------------------
# Specify output library and linker dependencies
#-------------------------------------------------------------------------------
add_library(signal SHARED ${SRC})
target_link_libraries(signal ${SNDFILE} ${SOUNDIO})
target_link_libraries(signal GSL::gsl GSL::gslcblas)
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(signal
        "-framework Accelerate"
        "-framework Accelerate"
        "-framework AppKit"
    )
endif()

if (CMAKE_BUILD_PYTHON)
    target_link_libraries(signal ${PYTHON_LIBRARY})
else()
    #-------------------------------------------------------------------------------
    # Build examples
    #-------------------------------------------------------------------------------
    add_subdirectory("examples")
endif()

