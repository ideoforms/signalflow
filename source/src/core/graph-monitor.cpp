#include "signal/core/graph-monitor.h"
#include <unistd.h>

namespace libsignal
{

AudioGraphMonitor::AudioGraphMonitor(AudioGraphRef graph, float frequency)
    : graph(graph), frequency(frequency)
{

}

void AudioGraphMonitor::start()
{
    this->running = true;
    this->thread = std::shared_ptr<std::thread>(new std::thread(&AudioGraphMonitor::run_thread, this));
}

void AudioGraphMonitor::stop()
{
    this->running = false;
}

void AudioGraphMonitor::run_thread()
{
    float sleep_time = 1e6 * (1.0 / this->frequency);
    while (this->running)
    {
        int node_count = this->graph->get_node_count();
        std::string nodes = (node_count == 1 ? "node" : "nodes");
        int patch_count = this->graph->get_patch_count();
        std::string patches = (patch_count == 1 ? "patch" : "patches");
        float cpu_usage = this->graph->get_cpu_usage() * 100.0;
        std::cout << "AudioGraph: " <<
            this->graph->get_node_count() << " active " << nodes << ", " <<
            this->graph->get_patch_count() << " " << patches << ", " <<
            cpu_usage << "% CPU usage" << std::endl;
        usleep(sleep_time);
    }
}

}
