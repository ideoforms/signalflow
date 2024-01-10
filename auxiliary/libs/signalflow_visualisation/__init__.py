import os
import json
import networkx as nx
from IPython.display import SVG
from signalflow import Patch

def visualise_patch_structure(patch: Patch, filename: str = None, dpi: int = None):
    """
    Renders the structure of a patch as a directed graph.

    Requires:
     - networkx
     - pygraphviz (https://github.com/pygraphviz/pygraphviz/issues/11)

    Args:
        patch (Patch): The patch to diagram.
        filename (str): If specified, writes the output to a file (can be .svg, .pdf, .png)
        dpi (int): If specified, overwrites the default DPI (which is 72 for screen, 300 for file)

    Returns:
        An IPython SVG object that can be rendered in a notebook.

    TODO: Implement support for cyclical graphs (requires reformulating JSON using JSON pointers)
    """

    G = nx.DiGraph()

    def label_map(label):
        lookup = {
            "add": "+",
            "multiply": "×",
            "subtract": "-",
            "divide": "÷"
        }
        if label in lookup.keys():
            return lookup[label]
        else:
            return label

    spec = patch.to_spec()
    structure = json.loads(spec.to_json())
    nodes = structure["nodes"]
    for node in nodes:
        node_label = node["node"]
        node_label = label_map(node_label)
        node_label = "<b>%s</b>" % node_label

        node_label += "<font point-size='8'><br />"
        for input_key, input_value in node["inputs"].items():
            if not isinstance(input_value, dict):
                node_label += "<br /><font point-size='2'><br /></font>%s = %s" % (input_key, round(input_value, 7))
        node_label += "</font>"

        # special graphviz syntax for enabling HTML formatting in node labels
        node_label = "<%s>" % node_label
        G.add_node(node["id"], label=node_label)
    for node in nodes:
        for input_key, input_value in node["inputs"].items():
            if isinstance(input_value, dict):
                label = ""
                if not input_key.startswith("input"):
                    label = input_key
                    # white background
                    # label = "<<table border='0' cellborder='0' cellspacing='0'><tr><td bgcolor='white'>%s</td></tr></table>>" % label
                G.add_edge(input_value["id"], node["id"], label=label)

    ag = nx.nx_agraph.to_agraph(G)
    ag.graph_attr["splines"] = "polyline"
    ag.node_attr["penwidth"] = 0.5
    ag.node_attr["fontname"] = "helvetica"
    ag.node_attr["fontsize"] = 9
    ag.node_attr["margin"] = 0.12
    ag.node_attr["height"] = 0.3
    ag.edge_attr["fontname"] = "helvetica"
    ag.edge_attr["fontsize"] = 8
    ag.edge_attr["penwidth"] = 0.5
    ag.edge_attr["arrowsize"] = 0.5
    ag.edge_attr["labelfloat"] = False
    ag.edge_attr["labeldistance"] = 0
    ag.node_attr["shape"] = "rectangle"
    ag.layout(prog='dot')

    if filename is not None:
        _, format = os.path.splitext(filename)
        format = format[1:]
        assert format in ["svg", "pdf", "png"]
        ag.graph_attr["dpi"] = dpi if dpi is not None else 300
        ag.draw(format=format,
                path=filename)
    else:
        ag.graph_attr["dpi"] = dpi if dpi is not None else 72
        svg = ag.draw(format='svg')
        return SVG(svg)