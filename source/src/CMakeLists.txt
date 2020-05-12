set(SRC ${SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/buffer/buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffer/buffer2D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/graph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/graph-monitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/random.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/stochastic/random-impulse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/stochastic/logistic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/stochastic/noise.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/stochastic/trigger-noise.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/envelope/adsr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/envelope/asr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/fft/fft.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/fft/fftnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/fft/ifft.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/fft/lpf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/fft/noise_gate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/fft/phase_vocoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/fft/zero_phase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/fft/tonality.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/delays/comb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/eq.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/freeze.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/gate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/moog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/pan.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/resample.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/rms.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/waveshaper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filters/width.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/io/input/abstract.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/io/input/soundio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/io/output/abstract.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/io/output/soundio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/node-monitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/operators/add.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/operators/divide.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/operators/mixer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/operators/multiplex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/operators/multiply.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/operators/round.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/operators/scale.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/operators/subtract.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/constant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/grainsegments.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/granulator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/line.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/saw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/segmentplayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/sine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/square.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/impulse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/triangle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/oscillators/wavetable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/buffer/buffer-recorder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/buffer/buffer-player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/registry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/sequencing/counter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/sequencing/index.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/patch/nodespec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/patch/patch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/patch/patchregistry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/patch/patchspec.cpp
)

if (CMAKE_BUILD_PYTHON)
    set(SRC ${SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/python/python.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/python/constants.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/python/node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/python/nodes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/python/graph.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/python/buffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/python/patch.cpp
    )
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(SRC ${SRC} 
        ${CMAKE_CURRENT_SOURCE_DIR}/node/control/mouse.mm
    )
endif()

set(SRC ${SRC} PARENT_SCOPE)
