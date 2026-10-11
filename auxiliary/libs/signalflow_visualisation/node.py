from signalflow import Node
import matplotlib.pyplot as plt

def plot_node_output(node: Node, duration: float = 1.0):
    buffer = node.graph.render_subgraph_to_new_buffer(node)
    plt.plot(buffer.data[0])
    plt.show()