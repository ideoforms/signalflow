#include "signalflow/core/graph-monitor.h"
#include <thread>

namespace signalflow
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
    float sleep_time = 1.0 / this->frequency;
    while (this->running)
    {
        std::string status = this->graph->get_status();
        std::cout << status << std::endl;
        std::this_thread::sleep_for(std::chrono::duration<float>(sleep_time));
    }
}

}
