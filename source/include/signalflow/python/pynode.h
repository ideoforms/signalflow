#pragma once

#include "signalflow/python/python.h"

/*--------------------------------------------------------------------------------
 * PyNode: Trampoline class that allows Node to be subclassed in Python.
 *
 * A Python subclass implements process(self, num_frames), which reads from
 * its inputs' output_buffer arrays and writes to self.output_buffer.
 *
 * Lifetime: pybind11 cannot keep the Python half of an object alive while only
 * C++ holds a reference to it (e.g. after `graph.play(MyNode())`), which would
 * lose the process() override and any Python state. Each PyNode therefore holds
 * a strong reference to its own Python object. sweep() releases these
 * self-references for nodes that are no longer referenced by either C++ or
 * Python, and is called whenever a new PyNode is created.
 *-------------------------------------------------------------------------------*/
class PyNode : public Node
{
public:
    using Node::Node;
    virtual ~PyNode();

    virtual void process(Buffer &out, int num_frames) override;

    void create_python_input(std::string name, NodeRef value);
    using Node::set_channels;

    /*--------------------------------------------------------------------------------
     * Retain the node's Python object. Called once the Python subclass's
     * __init__ has completed. Has no effect if already retained.
     *-------------------------------------------------------------------------------*/
    void retain(py::object self);

    /*--------------------------------------------------------------------------------
     * Release the self-reference of any node that is referenced only by its own
     * Python object, which in turn is the only owner of the C++ node.
     * Must be called with the GIL held.
     *-------------------------------------------------------------------------------*/
    static void sweep();

private:
    void zero_output(Buffer &out, int num_frames);

    std::map<std::string, NodeRef> input_storage;
    py::object self;
    bool is_processing = false;
};

/*--------------------------------------------------------------------------------
 * Returns true if the C++ Node underlying a Python Node object has been
 * constructed. This is false within a Python subclass's __init__ until it has
 * called super().__init__().
 *
 * Must be checked before casting such an object to Node: if the C++ object does
 * not yet exist, pybind11 allocates uninitialised memory for it and returns a
 * pointer to that, rather than failing.
 *-------------------------------------------------------------------------------*/
bool is_node_constructed(py::handle self);

/*--------------------------------------------------------------------------------
 * Adds the methods and hooks that support Python subclasses of Node.
 *-------------------------------------------------------------------------------*/
void init_python_pynode(py::class_<Node, PyNode, NodeRefTemplate<Node>> &node_class);
