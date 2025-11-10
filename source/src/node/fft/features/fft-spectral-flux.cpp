#include "signalflow/core/graph.h"
#include "signalflow/node/fft/features/fft-spectral-flux.h"
#include <algorithm>
#include <cmath>

namespace signalflow
{

FFTSpectralFlux::FFTSpectralFlux(NodeRef input)
    : FFTOpNode(input)
{
    this->name = "fft-spectral-flux";

    // Initialize previous magnitudes if needed
    previous_magnitudes.resize(this->num_bins, 0.0f);
}

void FFTSpectralFlux::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        double flux = 0.0;

        // Calculate spectral flux as sum of squared differences
        for (int frame = 0; frame < this->num_bins; frame++)
        {
            float current_mag = this->input->out[hop][frame];
            float diff = current_mag - previous_magnitudes[frame];

            flux += diff * diff;

            // Store current magnitude for next hop
            previous_magnitudes[frame] = current_mag;
        }

        // Take square root to get RMS-style flux
        float rv = sqrt(flux);

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            out[hop][frame] = rv;
        }
    }
}

}
