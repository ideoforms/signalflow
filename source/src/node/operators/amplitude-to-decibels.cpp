#include "signalflow/core/util.h"
#include "signalflow/node/operators/amplitude-to-decibels.h"

namespace signalflow
{

AmplitudeToDecibels::AmplitudeToDecibels(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "amplitude-to-decibels";
}

void AmplitudeToDecibels::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = signalflow_amplitude_to_db(this->input->out[channel][frame]);
        }
    }
}

DecibelsToAmplitude::DecibelsToAmplitude(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "decibels-to-amplitude";
}

void DecibelsToAmplitude::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = signalflow_db_to_amplitude(this->input->out[channel][frame]);
        }
    }
}

}
