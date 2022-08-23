cmake_minimum_required(VERSION 3.12.0)

#-------------------------------------------------------------------------------
# Build features
#-------------------------------------------------------------------------------
option(CMAKE_BUILD_SOUNDIO "Audio Hardware I/O support" ON)
if (CMAKE_BUILD_SOUNDIO)
    list(APPEND VCPKG_MANIFEST_FEATURES "soundio")
endif ()

option(CMAKE_BUILD_PYTHON "Build Python bindings" OFF)
if (CMAKE_BUILD_PYTHON)
    list(APPEND VCPKG_MANIFEST_FEATURES "python")
endif ()

option(CMAKE_BUILD_VAMP "Vamp plugin support" OFF)
if (CMAKE_BUILD_VAMP)
    list(APPEND VCPKG_MANIFEST_FEATURES "vamp")
endif ()

if (VCPKG_MANIFEST_FEATURES)
    message("Building with features:")
    foreach (feature IN LISTS VCPKG_MANIFEST_FEATURES)
        message("    - ${feature}")
    endforeach ()
else ()
    message("Building without any extra features")
endif ()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

#-------------------------------------------------------------------------------
# Project
#-------------------------------------------------------------------------------
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
# By default, build a universal library on Apple.
# For a faster build, select a single architecture by defining the
# environmental variable CMAKE_OSX_ARCHITECTURES.
# Must be set prior to `project()`
#-------------------------------------------------------------------------------
if (NOT CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

#-------------------------------------------------------------------------------
# Shared compiler flags.
#-------------------------------------------------------------------------------
add_compile_options(
    -pedantic
    -fPIC
    )

if (NOT MSVC)
    add_compile_options(
        -Wall
    )
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_compile_options(
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-vla-extension
    )
endif()

include_directories(
    source/include
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
# Specify source files.
# See source/CMakeLists.
#-------------------------------------------------------------------------------
set(SRC)
add_subdirectory("source/src")

#-------------------------------------------------------------------------------
# For builds of the Python bindings, add Python dependencies and flags
#-------------------------------------------------------------------------------
if (CMAKE_BUILD_PYTHON)
    find_package (Python3 COMPONENTS Interpreter Development)
    include_directories(${Python3_INCLUDE_DIRS})
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
        add_link_options(-w -undefined dynamic_lookup)
    endif()
endif()


#-------------------------------------------------------------------------------
# Specify output library
#-------------------------------------------------------------------------------
add_library(signalflow SHARED ${SRC})


#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------

if (CMAKE_BUILD_SOUNDIO)
    find_package(libsoundio CONFIG REQUIRED)
    if (libsoundio_FOUND)
        add_definitions(-DHAVE_SOUNDIO)
        target_link_libraries(signalflow libsoundio::libsoundio libsoundio::libsoundio_static)
    endif ()
endif ()

find_package(SndFile CONFIG REQUIRED)
if (SndFile_FOUND)
    add_definitions(-DHAVE_SNDFILE)
    target_link_libraries(signalflow SndFile::sndfile)
endif ()

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(FFTW_BUILD_DIR "" CACHE PATH "Path to prebuilt FFTW library (will use find_library if blank)")
    if (FFTW_BUILD_DIR)
        include_directories("${FFTW_BUILD_DIR}")
        add_definitions(-DFFT_FFTW)
        target_link_libraries(signalflow 
            "${FFTW_BUILD_DIR}/libfftw3-3.lib" 
            "${FFTW_BUILD_DIR}/libfftw3f-3.lib" 
            "${FFTW_BUILD_DIR}/libfftw3l-3.lib"
        )
    else()
        find_package(FFTW3f CONFIG REQUIRED)
        target_link_libraries(signalflow PRIVATE FFTW3::fftw3f)
        if (FFTW3f_FOUND)
            add_definitions(-DFFT_FFTW)
        endif ()
    endif()
endif(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")

if (CMAKE_BUILD_VAMP)
    find_library(VAMP vamp-hostsdk)
    if (VAMP)
        message("Found vamp")
        add_definitions(-DHAVE_VAMP)
        target_link_libraries(signalflow ${VAMP})
    else ()
        message(WARNING "Couldn't find vamp")
    endif ()
endif ()

find_library(JSON11 json11)
if (JSON11)
    target_link_libraries(signalflow ${JSON11})
else()
    message(WARNING "Couldn't find json11")
endif ()

if (CMAKE_BUILD_PYTHON)
    find_package(Python3 COMPONENTS Development REQUIRED)
    if (python3_FOUND)
        target_link_libraries(signalflow Python3::Python)
    else()
        message(WARNING "Couldn't find python3")
    endif ()

    find_package(pybind11 CONFIG REQUIRED)
    if (pybind11_FOUND)
        target_link_libraries(signalflow pybind11::lto pybind11::embed pybind11::module)
    else()
        message(WARNING "Couldn't find pybind11")
    endif ()
endif ()


#-------------------------------------------------------------------------------
# Specify additional linker dependencies
#-------------------------------------------------------------------------------
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(signalflow
        "-framework Accelerate"
        "-framework AppKit"
    )
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
