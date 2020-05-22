#!/usr/bin/env python3

#------------------------------------------------------------------------
# Automatically generate pybind11 headers to provide Python bindings
# for all Node subclasses.
#
# Requirements:
#
#   pip3 install robotpy-cppheaderparser
#------------------------------------------------------------------------

import os
import re
import sys
import glob
import pprint
import subprocess
import CppHeaderParser

top_level = subprocess.check_output([ "git", "rev-parse", "--show-toplevel" ]).decode().strip()
header_root = os.path.join(top_level, "source", "include")
source_files = glob.glob("%s/signal/node/*/*.h" % header_root) + glob.glob("%s/signal/node/*/*/*.h" % header_root)
source_files = list(filter(lambda path: not "/io/" in path, source_files))

def generate_class_bindings(class_name, parameter_sets):
    """
    py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
    .def(py::init<NodeRef>(),               "frequency"_a = NodeRef(440.0))
    .def(py::init<float>(),                 "frequency"_a = NodeRef(440.0))
    .def(py::init<std::vector<NodeRef>>(),  "frequency"_a = NodeRef(440.0))
    .def(py::init<std::vector<float>>(),    "frequency"_a = NodeRef(440.0));
    """
    if class_name in [ "VampAnalysis", "SegmentPlayer", "GrainSegments", "FFTNoiseGate", "FFTZeroPhase", "FFTOpNode", "FFTNode" ]:
        return ""

    output = 'py::class_<{class_name}, Node, NodeRefTemplate<{class_name}>>(m, "{class_name}")\n'.format(class_name=class_name)
    for parameter_set in parameter_sets:
        parameter_type_list = ", ".join([ parameter["type"] for parameter in parameter_set ])
        output += '    .def(py::init<{parameter_type_list}>()'.format(parameter_type_list=parameter_type_list);
        for parameter in parameter_set:
            if parameter["default"]:
                default = parameter["default"]
                # property defaults
                if default == "{}":
                    default = 0
                default = "%s" % default
                output += ', "{name}"_a = {default}'.format(name=parameter["name"], default=default)
            else:
                output += ', "{name}"_a'.format(name=parameter["name"])
        output += ')\n'
    output = output[:-1] + ";\n"
    return output

def generate_all_bindings():
    output = ""
    for source_file in source_files:
        try:
            header = CppHeaderParser.CppHeader(source_file)
        except CppHeaderParser.CppParseError as e:
            print(e)
            sys.exit(1)

        for class_name, value in header.classes.items():
            parent_class = None
            if "inherits" in value and len(value["inherits"]):
                parent_class = value["inherits"][0]["class"]
                if parent_class not in [ "Node", "UnaryOpNode", "BinaryOpNode", "FFTNode", "FFTOpNode" ]:
                    continue
            else:
                continue

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
                        if p_type in [ "sample", "double" ]:
                            p_type = "float"
                        if p_type in [ "string" ]:
                            p_type = "std::string"
                        parameters.append({
                                "type" : p_type,
                                "name" : p_name,
                                "default" : p_default
                                })
                    constructor_parameter_sets.append(parameters)
            if constructor_parameter_sets:
                output += generate_class_bindings(class_name, constructor_parameter_sets)
                output = output.strip()
                output += "\n\n"
    return output

bindings = generate_all_bindings()
bindings = re.sub("\n", "\n    ", bindings)
output = '''#include "signal/python/python.h"

void init_python_nodes(py::module &m)
{{
    /*--------------------------------------------------------------------------------
     * Node subclasses
     *-------------------------------------------------------------------------------*/
    {bindings}
}}
'''.format(bindings=bindings)

print(output)
