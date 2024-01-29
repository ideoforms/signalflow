#include "signalflow/node/node-monitor.h"

namespace signalflow
{

NodeMonitor::NodeMonitor(Node *node, std::string label, float frequency)
    : node(node), frequency(frequency), label(label)
{
    if (label == "")
        this->label = this->node->get_name();
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
    float sleep_time = 1.0f / this->frequency;
    char buffer[1024];
    while (this->running)
    {
        if (this->node->get_num_output_channels() > 1)
        {
            strcpy(buffer, "{ ");
            for (uint8_t i = 0; i < this->node->get_num_output_channels(); i++)
            {
                snprintf(buffer + strlen(buffer), 32, "%.5f", this->node->out[i][0]);
                if (i < this->node->get_num_output_channels() - 1)
                {
                    strcat(buffer, ", ");
                }
            }
            strcat(buffer, " }");
            fprintf(stderr, "%s: %s\n", this->label.c_str(), buffer);
        }
        else
        {
            fprintf(stderr, "%s: %.5f\n", this->label.c_str(), this->node->out[0][0]);
        }
        std::this_thread::sleep_for(std::chrono::duration<float>(sleep_time));
    }
}

void NodeMonitor::stop()
{
    this->running = false;
}

}
