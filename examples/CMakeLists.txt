set(EXAMPLES ${EXAMPLES}
    ${CMAKE_CURRENT_SOURCE_DIR}/audio-in-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio-write-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/granulator-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hello-world.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/index-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/json-load-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sine-field-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spooky-wobble.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/supersaw-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/synth-template-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/waveshaper-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wavetable-2d-example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wavetable-example.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(EXAMPLES ${EXAMPLES}
        ${CMAKE_CURRENT_SOURCE_DIR}/fft-lpf-example.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/fft-phase-vocoder-example.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/fft-sound-freeze.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mouse-control-example.cpp
    )
endif()

if (NOT CMAKE_BUILD_PYTHON)
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
        target_link_libraries(${EXAMPLE_NAME} signal)
    endforeach()
endif()
