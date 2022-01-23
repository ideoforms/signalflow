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
    this->thread = std::thread(&AudioGraphMonitor::run_thread, this);
    this->thread.detach();
}

void AudioGraphMonitor::stop()
{
    this->running = false;
}

bool AudioGraphMonitor::is_running()
{
    return this->running;
}

void AudioGraphMonitor::set_frequency(float frequency)
{
    this->frequency = frequency;
}

void AudioGraphMonitor::run_thread()
{
    while (this->running)
    {
        float sleep_time = 1.0 / this->frequency;
        std::string status = this->graph->get_status();
        std::cout << status << std::endl;
        std::this_thread::sleep_for(std::chrono::duration<float>(sleep_time));
    }
}

}
