cmake_minimum_required(VERSION 3.10.0)
project(Signal C CXX)

#-------------------------------------------------------------------------------
# Use C++11
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MACOSX_RPATH 1)

#-------------------------------------------------------------------------------
# Shared compiler flags
#-------------------------------------------------------------------------------
add_compile_options(
    -Wall
    -Wno-gnu-zero-variadic-macro-arguments
    -fPIC
    -pedantic)

include_directories(
    /usr/local/include
    source/include
    source/lib
    source/lib/pybind11/include
)

if (DEBUG)
    message("Building in debug mode")
    add_compile_options(-ggdb3 -O0)
else()
    message("Building in release mode")
    # add_compile_options(-O3 -funroll-loops)
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

find_library(GSL gsl)
if (GSL)
    message("Found libgls")
    add_definitions(-DHAVE_GSL)
else()
    message(FATAL_ERROR "Couldn't find libgsl")
endif()

#-------------------------------------------------------------------------------
# Specify source files and target libraries
#-------------------------------------------------------------------------------
set(SRC)
add_subdirectory("source/src")
set(SRC ${SRC} source/lib/json11/json11.cpp)

if (CMAKE_BUILD_PYTHON)
    set(SRC ${SRC} source/src/python/python.cpp)
    find_package (Python3 COMPONENTS Interpreter Development)
    include_directories(${Python3_INCLUDE_DIRS})
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
        add_link_options(-w -undefined dynamic_lookup)
    endif()
endif()

add_library(signal SHARED ${SRC})
target_link_libraries(signal ${GSL} ${SNDFILE} ${SOUNDIO})
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(signal
        "-framework Accelerate"
        "-framework Accelerate"
        "-framework AppKit"
    )
endif()

if (CMAKE_BUILD_PYTHON)
    target_link_libraries(signal ${Python3_LIBRARIES})
endif()

#-------------------------------------------------------------------------------
# Build examples
#-------------------------------------------------------------------------------
if (NOT CMAKE_BUILD_PYTHON)
    file(GLOB_RECURSE EXAMPLES "examples/*.cpp")

    foreach (EXAMPLE ${EXAMPLES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
        add_executable(${EXAMPLE_NAME} ${EXAMPLE})
        target_link_libraries(${EXAMPLE_NAME} signal)
    endforeach()
endif()
