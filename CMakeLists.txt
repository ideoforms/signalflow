#-------------------------------------------------------------------------------
# Require VERSION 3.15.0 is needed to correctly handle virtualenvs with
# FindPython3
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15.0)

project(SignalFlow C CXX)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Develop)
endif()

#-------------------------------------------------------------------------------
# By default, build arm64 on Apple.
#-------------------------------------------------------------------------------
if (NOT CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if (CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
        set(CMAKE_PREFIX_PATH /opt/homebrew)
    else()
        set(CMAKE_PREFIX_PATH /usr/local)
    endif()

    #--------------------------------------------------------------------------------
    # Allow deployment on older versions of macOS,
    # and default to the include/lib paths of the current Python virtualenv
    # (important for cross-compiling wheels)
    #--------------------------------------------------------------------------------
    # set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS deployment version" FORCE)
    set(Python_FIND_VIRTUALENV STANDARD)
    set(Python_FIND_FRAMEWORKS LAST)
endif()

#--------------------------------------------------------------------------------
# Print config setup to help with debugging
#--------------------------------------------------------------------------------
include(CMakePrintHelpers)
cmake_print_variables(CMAKE_SYSTEM_NAME)
cmake_print_variables(CMAKE_OSX_ARCHITECTURES)
cmake_print_variables(CMAKE_PREFIX_PATH)

#-------------------------------------------------------------------------------
# Use C++11
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MACOSX_RPATH 1)

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

#-------------------------------------------------------------------------------
# Hide superfluous compiler warnings on macOS
#-------------------------------------------------------------------------------
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_compile_options(
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-vla-extension
    )
endif()

include_directories(
    /usr/local/include
    /opt/homebrew/include
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
# Specify source files.
# See source/CMakeLists.
#-------------------------------------------------------------------------------
set(SRC)
add_subdirectory("source/src")
set(SRC ${SRC} source/lib/json11/json11.cpp)

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
# Pass SIGNALFLOW_VERSION from CMake flag to compiler
#-------------------------------------------------------------------------------
add_compile_definitions(SIGNALFLOW_VERSION="${SIGNALFLOW_VERSION}")

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------

set(SOUNDIO_BUILD_DIR "" CACHE PATH "Path to built SoundIO library (will use find_library if blank)")
if (SOUNDIO_BUILD_DIR)
    set(SOUNDIO_INCLUDE_DIR "${SOUNDIO_BUILD_DIR}/.." CACHE PATH "Path to SoundIO include directory (ignored if SOUNDIO_BUILD_DIR is blank")
    add_definitions(-DHAVE_SOUNDIO)
    target_link_libraries(signalflow "${SOUNDIO_BUILD_DIR}/$<CONFIG>/soundio.lib")
    include_directories(signalflow "${SOUNDIO_BUILD_DIR}/$<CONFIG>/")
    include_directories(signalflow "${SOUNDIO_INCLUDE_DIR}/")
else()
    find_library(SOUNDIO soundio)
    if (SOUNDIO)
        message("Found libsoundio")
        add_definitions(-DHAVE_SOUNDIO)
        target_link_libraries(signalflow ${SOUNDIO})
    else()
        message(SEND_ERROR "Couldn't find libsoundio")
    endif()
endif()


set(SNDFILE_BUILD_DIR "" CACHE PATH "Path to build sndfile library (will use find_library if blank)")
if (SNDFILE_BUILD_DIR)
    set(SNDFILE_INCLUDE_DIR "${SNDFILE_BUILD_DIR}/../include" CACHE PATH "Path to sndfile include directory (ignored if SNDFILE_BUILD_DIR is blank")
    add_definitions(-DHAVE_SNDFILE)
    target_link_libraries(signalflow "${SNDFILE_BUILD_DIR}/$<CONFIG>/sndfile.lib")
    include_directories(signalflow "${SNDFILE_BUILD_DIR}/include/")
    include_directories(signalflow "${SNDFILE_INCLUDE_DIR}/")
else()
    find_library(SNDFILE sndfile)
    if (SNDFILE)
        message("Found sndfile")
        add_definitions(-DHAVE_SNDFILE)
        target_link_libraries(signalflow ${SNDFILE})
        else()
        message(SEND_ERROR "Couldn't find libsndfile")
    endif()
endif()

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
        find_library(FFTW3F fftw3f)
        if (FFTW3F)
            message("Found fftw3f")
            target_link_libraries(signalflow ${FFTW3F})
            add_definitions(-DFFT_FFTW)
            else()
            # if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
                message(FATAL_ERROR "Couldn't find fftw3f")
            # endif()
        endif()
    endif()
endif(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")

find_library(VAMP vamp-hostsdk)
if (VAMP)
    message("Found vamp")
    add_definitions(-DHAVE_VAMP)
    target_link_libraries(signalflow ${VAMP})
else()
    message(WARNING "Couldn't find vamp")
endif()


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
