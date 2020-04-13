#include "node.h"

#include <unistd.h>
#include <thread>

namespace libsignal
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
        std::string label;
        bool running;
        std::shared_ptr<std::thread> thread;
};

}
