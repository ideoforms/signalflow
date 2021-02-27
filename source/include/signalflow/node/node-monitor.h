#include "signalflow/node/node.h"

#include <thread>
#include <unistd.h>

namespace signalflow
{

class NodeMonitor
{
    /**------------------------------------------------------------------------
 * NodeMonitor regularly polls the latest output value of a node,
 * and prints the first sample of the first output channel to stdout.
 * It creates its own thread for monitoring.
 *
 *-----------------------------------------------------------------------*/
public:
    /**------------------------------------------------------------------------
     * Create a NodeMonitor, attached to a specific node.
     *
     * @param node The node to monitor
     * @param label The label to use when printing the output
     * @param frequency How often to print, in seconds
     *-----------------------------------------------------------------------*/
    NodeMonitor(NodeRef node, std::string label, float frequency);

    /**------------------------------------------------------------------------
     * Start monitoring.
     *
     *-----------------------------------------------------------------------*/
    void start();

    /**------------------------------------------------------------------------
     * Stop monitoring.
     *
     *-----------------------------------------------------------------------*/
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
