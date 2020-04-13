#include "signal/monitor.h"

namespace libsignal
{

NodeMonitor::NodeMonitor(NodeRef node, std::string label, float frequency)
{
    this->node = node;
    this->frequency = frequency;

    if (label == "")
        this->label = this->node->name;
    else
        this->label = label;
}

void NodeMonitor::start()
{
    this->running = true;
    this->thread = std::shared_ptr<std::thread>(new std::thread(&NodeMonitor::run_thread, this));
}

void NodeMonitor::run_thread()
{
    float sleep_time = 1000000.0 * (1.0 / this->frequency);
    char buffer[1024];
    while (this->running)
    {
        if (this->node->num_output_channels > 1)
        {
            strcpy(buffer, "{ ");
            for (uint8_t i = 0; i < this->node->num_output_channels; i++)
            {
                sprintf(buffer + strlen(buffer), "%.5f", this->node->out[i][0]);
                if (i < this->node->num_output_channels - 1)
                {
                    sprintf(buffer + strlen(buffer), ", ");
                }
            }
            strcat(buffer, " }");
            fprintf(stderr, "%s: %s\n", this->label.c_str(), buffer);
        }
        else
        {
            fprintf(stderr, "%s: %.5f\n", this->label.c_str(), this->node->out[0][0]);
        }
        usleep(sleep_time);
    }
}

void NodeMonitor::stop()
{
    this->running = false;
}

}
