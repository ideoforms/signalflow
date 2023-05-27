#include "signalflow/python/python.h"

void init_python_graph(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Graph
     *-------------------------------------------------------------------------------*/
    py::class_<AudioGraph>(m, "AudioGraph")
        .def(py::init<AudioGraphConfig *, NodeRef, bool>(), "config"_a = nullptr, "output_device"_a = nullptr,
             "start"_a = true)

        /*--------------------------------------------------------------------------------
         * Properties
         *-------------------------------------------------------------------------------*/
        .def_property_readonly("config", &AudioGraph::get_config,
                               R"pbdoc(AudioGraphConfig: Get the current audio graph configuration.)pbdoc")
        .def_property_readonly("node_count", &AudioGraph::get_node_count,
                               R"pbdoc(int: Get the number of nodes currently active in the graph.)pbdoc")
        .def_property_readonly("patch_count", &AudioGraph::get_patch_count,
                               R"pbdoc(int: Get the number of patches currently active in the graph.)pbdoc")
        .def_property_readonly("cpu_usage", &AudioGraph::get_cpu_usage,
                               R"pbdoc(float: Get the current CPU usage (from 0.0 to 1.0).)pbdoc")
        .def_property_readonly("memory_usage", &AudioGraph::get_memory_usage,
                               R"pbdoc(int: Get the current memory usage, in bytes.)pbdoc")
        .def_property_readonly("num_output_channels", &AudioGraph::get_num_output_channels,
                               R"pbdoc(int: Get the number of output channels available in the graph.)pbdoc")
        .def_property_readonly("output_buffer_size", &AudioGraph::get_output_buffer_size,
                               R"pbdoc(int: Get the current output buffer size, in frames.)pbdoc")
        .def_property_readonly(
            "outputs", &AudioGraph::get_outputs,
            R"pbdoc(int: Get the list of Node objects currently connected to the graph's output.)pbdoc")
        .def_property_readonly("output_device_names", &AudioGraph::get_output_device_names,
                               R"pbdoc(list[str]: List the available output device names.)pbdoc")
        .def_property_readonly(
            "status", &AudioGraph::get_status,
            R"pbdoc(int: Get a text representation of the AudioGraph's status (node count, patch count, CPU usage).)pbdoc")
        .def_property_readonly(
            "structure", [](AudioGraph &graph) { return graph.get_structure(); },
            R"pbdoc(int: Get a text representation of the AudioGraph's node connectivity structure.)pbdoc")
        .def_property("sample_rate", &AudioGraph::get_sample_rate, &AudioGraph::set_sample_rate,
                      R"pbdoc(int: Get/set the graph's sample rate.)pbdoc")
        .def_property("output", &AudioGraph::get_output, &AudioGraph::set_output)

        /*--------------------------------------------------------------------------------
         * Methods
         *-------------------------------------------------------------------------------*/
        .def("start", &AudioGraph::start, R"pbdoc(Start the AudioGraph processing.)pbdoc")
        .def(
            "stop", [](AudioGraph &graph) { graph.stop(); }, R"pbdoc(Stop the AudioGraph processing.)pbdoc")
        .def("clear", &AudioGraph::clear,
             R"pbdoc(Remove all Node and Patches objects currently in the processing graph.)pbdoc")
        .def("destroy", &AudioGraph::destroy,
             R"pbdoc(Clear the AudioGraph and deallocate its memory, ready to create a new AudioGraph.)pbdoc")

        .def(
            "show_structure", [](AudioGraph &graph) { graph.show_structure(); },
            R"pbdoc(Print the AudioGraph's node connectivity structure to stdout.)pbdoc")
        .def(
            "poll", [](AudioGraph &graph, float frequency) { graph.poll(frequency); }, "frequency"_a,
            R"pbdoc(Begin polling the AudioGraph's status every `frequency` seconds, printing it to stdout.)pbdoc")
        .def(
            "poll", [](AudioGraph &graph) { graph.poll(); },
            R"pbdoc(Begin polling the AudioGraph's status every 1.0 seconds, printing it to stdout.)pbdoc")
        .def(
            "render", [](AudioGraph &graph) { graph.render(); },
            R"pbdoc(Render a single block (of `output_buffer_size` frames) of the AudioGraph's output.)pbdoc")
        .def(
            "render", [](AudioGraph &graph, int num_frames) { graph.render(num_frames); }, "num_frames"_a,
            R"pbdoc(Render a specified number of samples of the AudioGraph's output.)pbdoc")
        .def(
            "render_to_buffer", &AudioGraph::render_to_buffer, "buffer"_a,
            R"pbdoc(Render the graph's output to the specified buffer, for the same number of frames as the buffer's length.)pbdoc")
        .def(
            "render_to_new_buffer", &AudioGraph::render_to_new_buffer, "num_frames"_a,
            R"pbdoc(Render the graph's output for the specified number of frames, and return the resultant buffer.)pbdoc")
        .def(
            "render_subgraph",
            [](AudioGraph &graph, NodeRef node, int num_frames, bool reset) {
                if (reset)
                {
                    graph.reset_subgraph(node);
                }
                if (num_frames > 0)
                {
                    graph.render_subgraph(node, num_frames);
                }
                else
                {
                    graph.render_subgraph(node);
                }
            },
            "node"_a, "num_frames"_a = 0, "reset"_a = false,
            R"pbdoc(Recursively render the nodes in the tree starting at `node`. If `reset` is true, call `reset_subgraph` first.)pbdoc")
        .def("reset_subgraph", &AudioGraph::reset_subgraph,
             R"pbdoc(Reset the `played` status of nodes in the tree starting at `node`.)pbdoc")
        .def(
            "play", [](AudioGraph &graph, NodeRef node) { graph.play(node); }, "node"_a,
            R"pbdoc(Begin playback of `node` (by connecting it to the output of the graph))pbdoc")
        .def(
            "play", [](AudioGraph &graph, PatchRef patch) { graph.play(patch); }, "patch"_a,
            R"pbdoc(Begin playback of `patch` (by connecting it to the output of the graph))pbdoc")
        .def(
            "stop", [](AudioGraph &graph, NodeRef node) { graph.stop(node); }, "node"_a,
            R"pbdoc(Stop playback of `node` (by disconnecting it from the output of the graph))pbdoc")
        .def(
            "stop", [](AudioGraph &graph, PatchRef patch) { graph.stop(patch); }, "patch"_a,
            R"pbdoc(Stop playback of `patch]` (by disconnecting it from the output of the graph))pbdoc")
        .def(
            "replace", [](AudioGraph &graph, NodeRef node, NodeRef other) { graph.replace(node, other); }, "node"_a,
            "other"_a, R"pbdoc(Replace `node` in the graph's output with `other`.)pbdoc")
        .def(
            "add_node", &AudioGraph::add_node, "node"_a,
            R"pbdoc(Add `node` to the graph so that it is processed in future blocks, without connecting it to the graph's output. Useful for non-playback nodes (e.g. BufferRecorder).)pbdoc")
        .def("remove_node", &AudioGraph::remove_node, "node"_a,
             R"pbdoc(Remove a `node` that has previously been added with `add_node()`)pbdoc")

        .def(
            "start_recording", &AudioGraph::start_recording, "filename"_a = "", "num_channels"_a = 0,
            R"pbdoc(Start recording the graph's output to an audio file, with the same number of channels as the AudioGraph or `num_channels` if specified.)pbdoc")
        .def("stop_recording", &AudioGraph::stop_recording, R"pbdoc(Stop recording the graph's output.)pbdoc")

        .def("wait",
             [](AudioGraph &graph) {
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
        .def(
            "wait",
            [](AudioGraph &graph, float timeout_seconds) {
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
            },
            "seconds"_a);
}
