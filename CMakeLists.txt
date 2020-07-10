cmake_minimum_required(VERSION 3.10.0)
project(SignalFlow C CXX)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Develop)
endif()

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

find_library(FFTW3F fftw3f)
if (FFTW3F)
    message("Found fftw3f")
else()
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        message(FATAL_ERROR "Couldn't find fftw3f")
    endif()
endif()


#-------------------------------------------------------------------------------
# For builds of the Python bindings, add Python dependencies and flags
#-------------------------------------------------------------------------------
if (CMAKE_BUILD_PYTHON)
    find_package (PythonLibs 3)
    include_directories(${PYTHON_INCLUDE_DIR})
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CMAKE_SHARED_LIBRARY_PREFIX "")
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
add_library(signalflow SHARED ${SRC})
target_link_libraries(signalflow ${SNDFILE} ${SOUNDIO})
target_link_libraries(signalflow GSL::gsl GSL::gslcblas)
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(signalflow
        "-framework Accelerate"
        "-framework AppKit"
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(signalflow ${FFTW3F})
endif()

if (CMAKE_BUILD_PYTHON)
    target_link_libraries(signalflow ${PYTHON_LIBRARY})
else()
    #-------------------------------------------------------------------------------
    # Build examples
    #-------------------------------------------------------------------------------
    add_subdirectory("examples/cpp")
endif()

#-------------------------------------------------------------------------------
# Install shared lib and all includes
#-------------------------------------------------------------------------------
install(TARGETS signalflow DESTINATION lib)
install(DIRECTORY source/include/signalflow DESTINATION include)
