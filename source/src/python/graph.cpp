#include "signalflow/python/python.h"

void init_python_graph(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Graph
     *-------------------------------------------------------------------------------*/
    py::class_<AudioGraph>(m, "AudioGraph")
        .def(py::init<AudioGraphConfig *, NodeRef, bool>(),
             "config"_a = nullptr,
             "output_device"_a = nullptr,
             "start"_a = false)

        /*--------------------------------------------------------------------------------
         * Properties
         *-------------------------------------------------------------------------------*/
        .def_property_readonly("config", &AudioGraph::get_config)
        .def_property("sample_rate", &AudioGraph::get_sample_rate, &AudioGraph::set_sample_rate)
        .def_property_readonly("node_count", &AudioGraph::get_node_count)
        .def_property_readonly("cpu_usage", &AudioGraph::get_cpu_usage)
        .def_property_readonly("output", &AudioGraph::get_output)
        .def_property_readonly("outputs", &AudioGraph::get_outputs)
        .def_property_readonly("status", &AudioGraph::get_status)

        /*--------------------------------------------------------------------------------
         * Methods
         *-------------------------------------------------------------------------------*/
        .def("start", &AudioGraph::start)
        .def("stop", [](AudioGraph &graph) { graph.stop(); })
        .def("clear", &AudioGraph::clear)

        .def("show_structure", [](AudioGraph &graph) { graph.show_structure(); })
        .def("show_status", &AudioGraph::show_status)
        .def("render", [](AudioGraph &graph, int num_frames) { graph.render(num_frames); })
        .def("render_to_buffer", [](AudioGraph &graph, BufferRef buffer) { graph.render_to_buffer(buffer); })
        .def(
            "render_subgraph", [](AudioGraph &graph, NodeRef node, int num_frames, bool reset) {
                if (reset)
                {
                    graph.reset_subgraph(node);
                }
                graph.render_subgraph(node, num_frames);
            },
            "node"_a, "num_frames"_a = SIGNALFLOW_DEFAULT_BLOCK_SIZE, "reset"_a = false)
        .def("reset_subgraph", &AudioGraph::reset_subgraph)
        .def("play", [](AudioGraph &graph, NodeRef node) { graph.play(node); })
        .def("play", [](AudioGraph &graph, PatchRef patch) { graph.play(patch); })
        .def("stop", [](AudioGraph &graph, NodeRef node) { graph.stop(node); })
        .def("stop", [](AudioGraph &graph, PatchRef patch) { graph.stop(patch); })
        .def("replace", [](AudioGraph &graph, NodeRef node, NodeRef other) { graph.replace(node, other); })
        .def("add_node", &AudioGraph::add_node)
        .def("remove_node", &AudioGraph::remove_node)

        .def("start_recording", &AudioGraph::start_recording, "filename"_a = "", "num_channels"_a = 0)
        .def("stop_recording", &AudioGraph::stop_recording)

        .def("wait", [](AudioGraph &graph) {
            /*--------------------------------------------------------------------------------
             * Interruptible wait
             * https://pybind11.readthedocs.io/en/stable/faq.html#how-can-i-properly-handle-ctrl-c-in-long-running-functions
             *-------------------------------------------------------------------------------*/
            for (;;)
            {
                if (PyErr_CheckSignals() != 0)
                    throw py::error_already_set();
                /*--------------------------------------------------------------------------------
                 * Release the GIL so that other threads can do processing.
                 *-------------------------------------------------------------------------------*/
                py::gil_scoped_release release;
            }
        })
        .def("wait", [](AudioGraph &graph, float timeout_seconds) {
            timeval tv;
            gettimeofday(&tv, NULL);
            double t0 = tv.tv_sec + tv.tv_usec / 1000000.0;

            for (;;)
            {
                if (PyErr_CheckSignals() != 0)
                    throw py::error_already_set();

                if (timeout_seconds)
                {
                    gettimeofday(&tv, NULL);
                    double t1 = tv.tv_sec + tv.tv_usec / 1000000.0;
                    if (t1 - t0 > timeout_seconds)
                    {
                        break;
                    }
                }

                /*--------------------------------------------------------------------------------
                 * Release the GIL so that other threads can do processing.
                 *-------------------------------------------------------------------------------*/
                py::gil_scoped_release release;
            }
        });
}
