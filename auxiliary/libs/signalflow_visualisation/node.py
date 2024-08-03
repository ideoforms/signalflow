from signalflow import Node
import matplotlib.pyplot as plt
import numpy as np

def plot_node_output(node: Node, duration: float = 1.0):
    # TODO: Replace with graph.render_subgraph_to_new_buffer(node) when this is implemented
    num_frames = int(duration * node.graph.sample_rate)
    output = np.zeros(num_frames)
    num_chunks = int(np.ceil(num_frames / node.graph.output_buffer_size))
    for n in range(0, num_chunks):
        offset_start = n * node.graph.output_buffer_size
        offset_end = (n + 1) * node.graph.output_buffer_size
        offset_end = min(num_frames, offset_end)
        chunk_length = offset_end - offset_start
        node.graph.reset_subgraph(node)
        node.graph.render_subgraph(node)
        output[offset_start:offset_end] = node.output_buffer[0][:chunk_length]
    plt.plot(output)
    plt.show()