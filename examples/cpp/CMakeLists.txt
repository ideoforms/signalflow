set(EXAMPLES ${EXAMPLES}
    audio-in-example.cpp
    buffer-save-example.cpp
    granulator-example.cpp
    hello-world.cpp
    index-example.cpp
    json-load-example.cpp
    sine-field-example.cpp
    spooky-wobble.cpp
    supersaw-example.cpp
    patch-example.cpp
    trigger-example.cpp
    wavetable-2d-example.cpp
    wavetable-example.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(EXAMPLES ${EXAMPLES}
        mouse-control-example.cpp
    )
endif()

if (VAMP)
    set(EXAMPLES ${EXAMPLES}
            vamp-input-example.cpp
    )
endif()

foreach (EXAMPLE ${EXAMPLES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE})

    #------------------------------------------------------------------------
    # Don't build examples in an examples subdirectory of build
    #------------------------------------------------------------------------
    set_target_properties(${EXAMPLE_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    target_link_libraries(${EXAMPLE_NAME} signalflow)
endforeach()

if (WIN32 AND FFTW_BUILD_DIR)
    file(COPY "${FFTW_BUILD_DIR}/libfftw3-3.dll" DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY "${FFTW_BUILD_DIR}/libfftw3f-3.dll" DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY "${FFTW_BUILD_DIR}/libfftw3l-3.dll" DESTINATION ${CMAKE_BINARY_DIR})
endif()

