#include "signalflow/core/graph.h"

#include <memory>
#include <thread>

namespace signalflow
{

class AudioGraphMonitor
{
public:
    AudioGraphMonitor(AudioGraphRef graph, float frequency);

    void start();
    void stop();
    void set_frequency(float frequency);
    bool is_running();

private:
    void run_thread();
    AudioGraphRef graph;
    float frequency;
    bool running;
    std::thread thread;
};

}
