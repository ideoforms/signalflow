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

private:
    void run_thread();
    AudioGraphRef graph;
    float frequency;
    bool running;
    std::shared_ptr<std::thread> thread;
};

}
