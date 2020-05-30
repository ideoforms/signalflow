#ifdef __APPLE__

#include "signal/node/fft/find-peaks.h"
#include "signal/core/graph.h"

namespace libsignal
{

class Peak
{
public:
    Peak() {}
    Peak(float frequency, float magnitude) : frequency(frequency), magnitude(magnitude) {}
    float frequency;
    float magnitude;
};


int find_prev_bin_with_magnitude(sample *bins, int index)
{
    sample magnitude = bins[index];
    for (int bin_index = index - 1; bin_index > 1; bin_index--)
    {
        if (bins[bin_index] >= magnitude)
        {
            return bin_index;
        }
    }
    return 0;
}

int find_next_bin_with_magnitude(sample *bins, int index, int num_bins)
{
    sample magnitude = bins[index];
    for (int bin_index = index + 1; bin_index < num_bins; bin_index++)
    {
        if (bins[bin_index] >= magnitude)
        {
            return bin_index;
        }
    }
    return num_bins - 1;
}

sample get_min_magnitude_in_range(sample *bins, int index_left, int index_right)
{
    sample min_magnitude = 1e6;
    for (int bin_index = index_left; bin_index < index_right; bin_index++)
    {
        if (bins[bin_index] < min_magnitude)
        {
            min_magnitude = bins[bin_index];
        }
    }
    return min_magnitude;
}

FFTFindPeaks::FFTFindPeaks(NodeRef input, NodeRef prominence, NodeRef threshold, int count, bool interpolate)
    : FFTOpNode(input), prominence(prominence), threshold(threshold), count(count), interpolate(interpolate)
{
    this->name = "fft-find-peaks";
    this->num_output_channels = this->min_output_channels = this->max_output_channels = count;
    this->update_channels();

    this->add_input("prominence", this->prominence);
    this->add_input("threshold", this->threshold);
}

void FFTFindPeaks::process(sample **out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    std::vector <Peak>peaks(this->num_bins);
    peaks.clear();
    int peak_count = 0;

    for (int hop = 0; hop < 1; hop++)
    {
        sample *mags_in = this->input->out[hop];
        for (int bin_index = 2; bin_index < this->num_bins - 1; bin_index++)
        {
            if (mags_in[bin_index] > this->threshold->out[0][0] &&
                mags_in[bin_index] > mags_in[bin_index - 1] &&
                mags_in[bin_index] > mags_in[bin_index + 1])
            {
                // is a peak above threshold
                int index_left = find_prev_bin_with_magnitude(mags_in, bin_index);
                int index_right = find_next_bin_with_magnitude(mags_in, bin_index, this->num_bins);
                sample min_contour_left = get_min_magnitude_in_range(mags_in, index_left, bin_index);
                sample min_contour_right = get_min_magnitude_in_range(mags_in, bin_index + 1, index_right);
                sample max_min_contour = MAX(min_contour_left, min_contour_right);
                if (max_min_contour == 0)
                    max_min_contour = 1e-9;
                sample prominence = mags_in[bin_index] / max_min_contour;
                if (prominence > this->prominence->out[0][0] && peak_count < count)
                {
                    float p = 0.0;
                    if (this->interpolate)
                    {
                        float alpha = signal_amp_to_db(mags_in[bin_index - 1]);
                        float beta = signal_amp_to_db(mags_in[bin_index]);
                        float gamma = signal_amp_to_db(mags_in[bin_index + 1]);
                        p = (alpha == beta && beta == gamma) ? 0.0f : 0.5f * (alpha - gamma) / (alpha - 2.0f * beta + gamma);
                    }
                    float frequency = (bin_index + p) * graph->get_sample_rate() / this->fft_size;
                    peaks[peak_count++] = Peak(frequency, mags_in[bin_index]);
                }
            }
        }

        std::sort (peaks.begin(), peaks.begin() + peak_count, [](const Peak & a, const Peak & b) -> bool
        {
            return a.magnitude > b.magnitude;
        });

        for (int channel = 0; channel < count; channel++)
        {
            if (channel < peak_count)
            {
                int peak_index = channel;
                for (int bin_index = 0; bin_index < num_frames; bin_index++)
                {
                    out[peak_index][bin_index] = peaks[peak_index].frequency;
                }
            }
            else
            {
                for (int bin_index = 0; bin_index < num_frames; bin_index++)
                {
                    out[channel][bin_index] = 0;
                }
            }
        }
    }
}



}


#endif
