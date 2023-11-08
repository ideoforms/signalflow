#!/usr/bin/env python3

# ------------------------------------------------------------------------
# Automatically generate pybind11 headers to provide Python bindings
# for all Node subclasses.
#
# Usage:
#
#   auxiliary/scripts/generate-node-python-bindings.py > source/src/python/nodes.cpp
#
# Requirements:
#
#   pip3 install robotpy-cppheaderparser
#
# ------------------------------------------------------------------------

import os
import re
import sys
import glob
import argparse
import subprocess
import CppHeaderParser

parser = argparse.ArgumentParser()
parser.add_argument("-m", "--markdown", action="store_true")
parser.add_argument("-t", "--table", action="store_true")
args = parser.parse_args()

node_superclasses = ["Node", "UnaryOpNode", "BinaryOpNode", "StochasticNode", "FFTNode", "FFTOpNode", "LFO"]
omitted_classes = ["VampAnalysis", "GrainSegments", "FFTNoiseGate", "FFTZeroPhase", "FFTOpNode", "FFTNode",
                   "StochasticNode"]
macos_only_classes = ["MouseX", "MouseY", "MouseDown", "FFTConvolve"]

top_level = subprocess.check_output(["git", "rev-parse", "--show-toplevel"]).decode().strip()
header_root = os.path.join(top_level, "source", "include")
source_files = glob.glob("%s/signalflow/node/*/*.h" % header_root) + glob.glob(
    "%s/signalflow/node/*/*/*.h" % header_root)
source_files = list(filter(lambda path: not "/io/" in path, source_files))
source_files = list(sorted(source_files, key=lambda path: (os.path.dirname(path), path)))


def generate_class_bindings(class_name, parameter_sets, superclass="Node", class_docs=None):
    """
    py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
    .def(py::init<NodeRef>(),               "frequency"_a = NodeRef(440.0))
    .def(py::init<float>(),                 "frequency"_a = NodeRef(440.0))
    .def(py::init<std::vector<NodeRef>>(),  "frequency"_a = NodeRef(440.0))
    .def(py::init<std::vector<float>>(),    "frequency"_a = NodeRef(440.0));
    """
    if class_name in omitted_classes:
        return ""
    if class_docs is None:
        class_docs = class_name

    output = 'py::class_<{class_name}, {superclass}, NodeRefTemplate<{class_name}>>(m, "{class_name}", "{class_docs}")\n'.format(
        class_name=class_name, class_docs=class_docs, superclass=superclass)
    for parameter_set in parameter_sets:
        parameter_type_list = ", ".join([parameter["type"] for parameter in parameter_set])
        output += '    .def(py::init<{parameter_type_list}>()'.format(parameter_type_list=parameter_type_list);
        for parameter in parameter_set:
            if parameter["default"] is not None:
                default = parameter["default"]
                # property defaults
                if default == "{}":
                    default = 0
                elif default == "":
                    default = '""'
                default = "%s" % default
                output += ', "{name}"_a = {default}'.format(name=parameter["name"], default=default)
            else:
                output += ', "{name}"_a'.format(name=parameter["name"])
        output += ')\n'
    output = output[:-1] + ";\n"
    return output

def generate_all_bindings():
    output_markdown = ""
    folder_last = ""
    output = ""
    output += generate_class_bindings("AudioIn", [[]]) + "\n"
    output += generate_class_bindings("AudioOut_Abstract", []) + "\n"
    output += generate_class_bindings("AudioOut_Dummy", [[
        {"name": "num_channels", "type": "int", "default": 2},
        {"name": "buffer_size", "type": "int", "default": "SIGNALFLOW_DEFAULT_BLOCK_SIZE"},
    ]], "AudioOut_Abstract") + "\n"

    output += generate_class_bindings("AudioOut", [[
        {"name": "backend_name", "type": "std::string", "default": ""},
        {"name": "device_name", "type": "std::string", "default": ""},
        {"name": "sample_rate", "type": "int", "default": 0},
        {"name": "buffer_size", "type": "int", "default": 0}
    ]], "AudioOut_Abstract") + "\n"

    class_categories = {}
    class_category = None

    def folder_name_to_title(folder_name):
        folder_parts = [part.title() for part in folder_name.split("/")]
        folder_title = ": ".join(folder_parts)
        return folder_title

    for source_file in source_files:
        folder = re.sub(".*node/", "", source_file)
        folder = os.path.dirname(folder)
        if folder != folder_last:
            folder_title = folder_name_to_title(folder)
            output_markdown += "\n## " + folder_title + "\n\n"
            folder_last = folder
            class_category = folder
            class_categories[class_category] = []

        try:
            header = CppHeaderParser.CppHeader(source_file)
        except CppHeaderParser.CppParseError as e:
            print(e)
            sys.exit(1)

        for class_name, value in header.classes.items():
            if "inherits" in value and len(value["inherits"]):
                parent_class = value["inherits"][0]["class"]
                if parent_class not in node_superclasses:
                    continue
            else:
                continue


            def extract_docs(doxygen):
                lines = doxygen.split("\n")
                output = ""
                for line in lines:
                    # start or end of comment
                    if re.search(r"^\s*/\*", line) or re.search("\*/\s*$", line):
                        continue
                    line = re.sub(r"^\s*\*\s*", "", line)
                    output = output + line + " "
                return output.strip()

            class_docs = class_name
            if "doxygen" in value:
                class_docs = extract_docs(value["doxygen"])

            constructor_parameter_sets = []
            for method in value["methods"]["public"]:
                if method["constructor"]:
                    parameters = []
                    for parameter in method["parameters"]:
                        p_type = parameter["type"]
                        p_name = parameter["name"]
                        p_default = None
                        if "defaultValue" in parameter:
                            p_default = parameter["defaultValue"]
                        if p_type in ["sample", "double"]:
                            p_type = "float"
                        if p_type in ["string"]:
                            p_type = "std::string"
                        parameters.append({
                            "type": p_type,
                            "name": p_name,
                            "default": p_default
                        })
                    constructor_parameter_sets.append(parameters)
            if constructor_parameter_sets:
                if class_name in macos_only_classes:
                    output += "#ifdef __APPLE__\n\n"
                known_parent_classes = ["Node", "StochasticNode"]
                if parent_class not in known_parent_classes:
                    parent_class = "Node"
                output += generate_class_bindings(class_name, constructor_parameter_sets, parent_class, class_docs=class_docs)
                output = output.strip()
                output += "\n\n"
                if class_name in macos_only_classes:
                    output += "#endif\n\n"

                output_markdown_params = ", ".join(
                    ("%s=%s" % (param["name"], param["default"])) for param in constructor_parameter_sets[0])
                output_markdown += "- **%s**: %s `(%s)`\n" % (class_name, class_docs, output_markdown_params)
                class_categories[class_category].append(class_name)
    return output, output_markdown, class_categories


bindings, markdown, class_categories = generate_all_bindings()
bindings = re.sub("\n", "\n    ", bindings)
output = '''#include "signalflow/python/python.h"

void init_python_nodes(py::module &m)
{{
    /*--------------------------------------------------------------------------------
     * Node subclasses
     *-------------------------------------------------------------------------------*/
    {bindings}
}}
'''.format(bindings=bindings)

if args.markdown:
    print("# Node reference library")
    print()
    print(markdown)
elif args.table:
    output_table = "| Category | Classes  |\n"
    output_table += "|:---------|:---------|\n"
    for category, classes in class_categories.items():
        category_text = ": ".join(text.title() for text in category.split("/"))
        output_table += "| **%s** | %s |\n" % (category_text, ", ".join(classes))
    print(output_table)
else:
    print(output)
