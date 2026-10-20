#include "signalflow/python/pynode.h"

#include <atomic>
#include <cstring>
#include <set>

/*--------------------------------------------------------------------------------
 * Set at interpreter exit, after which Python Node subclasses output silence
 * rather than attempting to acquire the GIL from the audio thread.
 *-------------------------------------------------------------------------------*/
static std::atomic<bool> python_node_finalizing(false);

/*--------------------------------------------------------------------------------
 * PyNodes that currently hold a reference to their own Python object.
 * Only accessed with the GIL held.
 *-------------------------------------------------------------------------------*/
static std::set<PyNode *> retained_nodes;
static bool has_created_python_nodes = false;

PyNode::~PyNode()
{
    retained_nodes.erase(this);
}

void PyNode::process(Buffer &out, int num_frames)
{
    if (python_node_finalizing || this->is_processing)
    {
        this->zero_output(out, num_frames);
        return;
    }

    /*--------------------------------------------------------------------------------
     * Offline rendering (e.g. graph.render_subgraph()) is called from a Python
     * thread that already holds the GIL, in which case Python exceptions can be
     * propagated to the caller. On the audio thread, they cannot.
     *-------------------------------------------------------------------------------*/
    bool called_from_python = PyGILState_Check();
    std::string error_message;

    {
        py::gil_scoped_acquire acquire;
        py::function python_process_implementation = py::get_override(static_cast<const Node *>(this), "process");
        if (!python_process_implementation)
        {
            return;
        }

        this->is_processing = true;
        try
        {
            if (&out == &this->out)
            {
                python_process_implementation(num_frames);
            }
            else
            {
                /*--------------------------------------------------------------------------------
                 * Python code always renders into the node's own output buffer, so when
                 * rendering into an external buffer, render in chunks and copy.
                 *-------------------------------------------------------------------------------*/
                for (int offset = 0; offset < num_frames; offset += this->output_buffer_length)
                {
                    int chunk_frames = std::min(this->output_buffer_length, num_frames - offset);
                    python_process_implementation(chunk_frames);
                    for (int channel = 0; channel < this->num_output_channels; channel++)
                    {
                        memcpy(out[channel] + offset, this->out[channel], chunk_frames * sizeof(sample));
                    }
                }
            }
        }
        catch (py::error_already_set &e)
        {
            this->is_processing = false;
            if (called_from_python)
            {
                throw;
            }
            error_message = this->name + ": " + e.what();
        }
        this->is_processing = false;
    }

    if (!error_message.empty())
    {
        /*--------------------------------------------------------------------------------
         * Called after releasing the GIL, as this stops the audio device.
         *-------------------------------------------------------------------------------*/
        this->zero_output(out, num_frames);
        signalflow_audio_thread_error(error_message);
    }
}

void PyNode::create_python_input(std::string name, NodeRef value)
{
    if (this->inputs.find(name) != this->inputs.end())
    {
        throw std::runtime_error("Node " + this->name + " already has an input named: " + name);
    }

    /*--------------------------------------------------------------------------------
     * Node::create_input() stores a pointer to the NodeRef, so it must have a
     * stable address. std::map never relocates its elements.
     *-------------------------------------------------------------------------------*/
    this->input_storage[name] = value;
    this->Node::create_input(name, this->input_storage[name]);
}

void PyNode::retain(py::object self)
{
    if (retained_nodes.count(this))
    {
        return;
    }

    PyNode::sweep();
    this->self = self;
    this->name = py::str(self.get_type().attr("__name__"));
    retained_nodes.insert(this);
    has_created_python_nodes = true;
}

void PyNode::sweep()
{
    /*--------------------------------------------------------------------------------
     * sweep() is required for garbage collection, instructing the system to release
     * unreferenced Python nodes.
     *-------------------------------------------------------------------------------*/
    std::vector<py::object> released;
    for (auto it = retained_nodes.begin(); it != retained_nodes.end();)
    {
        /*--------------------------------------------------------------------------------
         * A node is unreferenced if its Python refcount is 1 (only the node's own
         * self-reference) and its holder's use_count is 1 (only the Python object).
         * The temporary holder copy here accounts for one further use.
         *-------------------------------------------------------------------------------*/
        PyNode *node = *it;
        bool unreferenced = false;
        if (node->self.ref_count() == 1)
        {
            NodeRef ref = node->self.cast<NodeRef>();
            unreferenced = (ref.use_count() == 2);
        }

        if (unreferenced)
        {
            released.push_back(std::move(node->self));
            it = retained_nodes.erase(it);
        }
        else
        {
            ++it;
        }
    }

    /*--------------------------------------------------------------------------------
     * Nodes are deallocated here, when `released` goes out of scope.
     *-------------------------------------------------------------------------------*/
}

void PyNode::zero_output(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        memset(out[channel], 0, num_frames * sizeof(sample));
    }
}

bool is_node_constructed(py::handle self)
{
    auto *instance = reinterpret_cast<py::detail::instance *>(self.ptr());
    return instance->get_value_and_holder().holder_constructed();
}

/*--------------------------------------------------------------------------------
 * Returns the PyNode underlying a Python subclass of Node, raising an exception
 * if super().__init__() has not yet been called, or if `self` is not a Python
 * subclass.
 *-------------------------------------------------------------------------------*/
static PyNode *get_py_node(py::object self, std::string method_name)
{
    if (!is_node_constructed(self))
    {
        throw std::runtime_error(method_name + "() cannot be called before super().__init__()");
    }
    PyNode *py_node = dynamic_cast<PyNode *>(self.cast<Node *>());
    if (!py_node)
    {
        throw std::runtime_error(method_name + "() can only be called on Python subclasses of Node");
    }
    return py_node;
}

void init_python_pynode(py::class_<Node, PyNode, NodeRefTemplate<Node>> &node_class)
{
    /*--------------------------------------------------------------------------------
     * Methods for use by Python subclasses of Node.
     *-------------------------------------------------------------------------------*/
    node_class
        .def(
            "create_input", [](py::object self, std::string name, NodeRef value) {
                get_py_node(self, "create_input")->create_python_input(name, value);
            },
            "name"_a, "value"_a = nullptr, R"pbdoc(Create a new audio-rate input named `name`, with initial value `value`. For use in the constructor of Python subclasses of Node.)pbdoc")
        .def(
            "set_channels", [](py::object self, int num_input_channels, int num_output_channels) {
                get_py_node(self, "set_channels")->set_channels(num_input_channels, num_output_channels);
            },
            "num_input_channels"_a, "num_output_channels"_a, R"pbdoc(Set a fixed number of input and output channels, disabling automatic matching of input channel counts. For use in the constructor of Python subclasses of Node.)pbdoc");

    /*--------------------------------------------------------------------------------
     * When a Python subclass of Node is defined, wrap its __init__ so that the
     * resultant PyNode can retain its Python object once constructed.
     * See PyNode for details.
     *-------------------------------------------------------------------------------*/
    py::object init_subclass = py::cpp_function([node_class](py::object cls, py::kwargs kwargs) {
        py::object subclass_init = cls.attr("__init__");
        cls.attr("__init__") = py::cpp_function(
            [subclass_init](py::object self, py::args args, py::kwargs kwargs) {
                subclass_init(self, *args, **kwargs);
                if (!is_node_constructed(self))
                {
                    std::string class_name = py::str(self.get_type().attr("__name__"));
                    throw py::type_error(class_name + ".__init__() must call super().__init__()");
                }
                PyNode *node = dynamic_cast<PyNode *>(self.cast<Node *>());
                if (node)
                {
                    node->retain(self);
                }
            },
            py::name("__init__"), py::is_method(cls));
        py::module::import("builtins").attr("super")(node_class, cls).attr("__init_subclass__")(**kwargs);
    });
    node_class.attr("__init_subclass__") = py::reinterpret_steal<py::object>(PyClassMethod_New(init_subclass.ptr()));

    /*--------------------------------------------------------------------------------
     * At interpreter exit, stop Python nodes from acquiring the GIL in the audio
     * thread, and stop the audio device (with the GIL released, so that any audio
     * callback waiting on the GIL can complete). Otherwise, the audio thread may
     * block indefinitely once interpreter finalisation begins.
     *-------------------------------------------------------------------------------*/
    py::module::import("atexit").attr("register")(py::cpp_function([]() {
        python_node_finalizing = true;
        AudioGraph *graph = AudioGraph::get_shared_graph();
        if (has_created_python_nodes && graph)
        {
            py::gil_scoped_release release;
            graph->stop();
        }
    }));
}
