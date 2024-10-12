#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Physically-inspired model of a maraca.
 *
 * Parameters:
 *  - `num_beans`: The number of simulated beans in the maraca (1-1024)
 *  - `shake_decay`: Decay constant for the energy injected per shake
 *  - `grain_decay`: Decay constant for the energy created per bean collision
 *  - `shake_duration`: Duration of each shake action, milliseconds
 *  - `shell_frequency`: Resonant frequency of the maraca's shell, hertz
 *  - `shell_resonance`: Resonanc of the maraca's shell (0-1)
 *  - `clock`: If specified, triggers shake actions
 *  - `energy`: If specified, injects energy into the maraca
 *
 * From Cook (1997), "Physically Informed Sonic Modeling (PhISM): Synthesis of
 * Percussive Sounds", Computer Music Journal.
 *---------------------------------------------------------------------------------*/
class Maraca : public StochasticNode
{
public:
    Maraca(NodeRef num_beans = 64,
           NodeRef shake_decay = 0.99,
           NodeRef grain_decay = 0.99,
           NodeRef shake_duration = 0.02,
           NodeRef shell_frequency = 12000,
           NodeRef shell_resonance = 0.9,
           NodeRef clock = nullptr,
           NodeRef energy = nullptr);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = SIGNALFLOW_NULL_FLOAT) override;

private:
    NodeRef num_beans;
    NodeRef shake_decay;
    NodeRef grain_decay;
    NodeRef shake_duration;
    NodeRef shell_frequency;
    NodeRef shell_resonance;
    NodeRef clock;
    NodeRef energy;
    BufferRef envelope;

    float shake_energy = 0.0;
    float shake_intensity = 1.0;
    float sound_level = 0.0;
    float coefs[2] = { 0, 0 };
    float output[2] = { 0, 0 };
    double phase = -1.0;
};

REGISTER(Maraca, "maraca")
}
