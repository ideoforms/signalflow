#include "signalflow/node/node.h"

#include <thread>
#include <unistd.h>

namespace signalflow
{

class NodeMonitor
{
public:
    NodeMonitor(NodeRef node, std::string label, float frequency);

    void start();
    void stop();

private:
    void run_thread();
    NodeRef node;
    float frequency;
    bool running;
    std::string label;
    std::shared_ptr<std::thread> thread;
};

}
