#include "node.h"

#include <unistd.h>
#include <thread>

namespace libsignal
{

class NodeMonitor
{
    public:
        NodeMonitor(Node *node, std::string label, float frequency);

        void start();
        void stop();

    private:
        void run_thread();
        Node *node;
        float frequency;
        std::string label;
        bool running;
        std::shared_ptr<std::thread> thread;
};

}
