#include "signalflow/core/core.h"
#include "signalflow/node/operators/select-input.h"

namespace signalflow
{
void SelectInput::init(NodeRef index)
{
    this->name = "select-input";

    this->index = index;
    this->create_input("index", this->index);
}

SelectInput::SelectInput(NodeRef index)
    : VariableInputNode()
{
    this->init(index);
}

SelectInput::SelectInput(std::initializer_list<NodeRef> inputs, NodeRef index)
    : VariableInputNode(inputs)
{
    this->init(index);
}

SelectInput::SelectInput(std::vector<NodeRef> inputs, NodeRef index)
    : VariableInputNode(inputs)
{
    this->init(index);
}

SelectInput::SelectInput(std::vector<float> inputs, NodeRef index)
    : VariableInputNode(inputs)
{
    this->init(index);
}

SelectInput::SelectInput(std::vector<int> inputs, NodeRef index)
    : VariableInputNode(inputs)
{
    this->init(index);
}

void SelectInput::process(Buffer &out, int num_frames)
{
    if (this->input_list.size() == 0)
    {
        signalflow_audio_thread_error("SelectInput: No inputs were passed to select from");
    }

    int index = 0;
    int last_index = -1;
    NodeRef selected_node = nullptr;
    for (int frame = 0; frame < num_frames; frame++)
    {
        index = ((int) this->index->out[0][frame]) % this->input_list.size();
        if (index != last_index)
        {
            selected_node = *std::next(this->input_list.begin(), index);
            last_index = index;
        }
        for (int channel = 0; channel < this->get_num_output_channels(); channel++)
        {
            out[channel][frame] = selected_node->out[channel][frame];
        }
    }
}

}
