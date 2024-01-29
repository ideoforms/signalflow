#!/usr/bin/env python3

# ------------------------------------------------------------------------
# Automatically parses the SignalFlow source tree and generates:
#
#  - Python bindings
#  - Documentation: Node class library
#  - Documentation: Node table for README
#
# Requirements:
#
#   pip3 install robotpy-cppheaderparser
# ------------------------------------------------------------------------

import os
import re
import glob
import argparse
import subprocess
import CppHeaderParser
from typing import Any, Optional
from dataclasses import dataclass


@dataclass
class Parameter:
    name: str
    type: str
    default: Any


@dataclass
class NodeClass:
    name: str
    parent: Optional[str]
    constructors: list[list[Parameter]]
    docs: str

    @property
    def identifier(self):
        return self.name.lower()


node_superclasses = ["Node", "UnaryOpNode", "BinaryOpNode", "StochasticNode", "FFTNode", "FFTOpNode", "LFO"]
omitted_classes = ["GrainSegments", "FFTZeroPhase", "FFTOpNode", "FFTNode",
                   "StochasticNode"]
macos_only_classes = ["MouseX", "MouseY", "MouseDown", "FFTConvolve"]
vamp_only_classes = ["VampAnalysis"]
known_parent_classes = ["Node", "StochasticNode"]
documentation_omit_folders = ["io"]
documentation_omit_classes = ["Constant"]

bindings_output_path = "source/src/python/nodes.cpp"


def get_all_source_files() -> list[str]:
    """
    Generate a list of all SignalFlow header files.

    Returns:
        A list of all SignalFlow header files.
    """
    header_root = os.path.join("source", "include")
    source_files = glob.glob("%s/signalflow/node/*/*.h" % header_root)
    source_files += glob.glob("%s/signalflow/node/*/*/*.h" % header_root)
    source_files = list(filter(lambda path: not "/io/" in path, source_files))
    source_files = list(sorted(source_files, key=lambda path: (os.path.dirname(path), path)))
    return source_files


def generate_class_bindings(cls: NodeClass):
    """
    Args:
        class_name: The full name of the C++ class
        parameter_sets: List of list of parameters accepted by the class's constructor
        superclass: The class's parent class
        class_docs: Docstring for the class

    Returns:
        A string containing valid pybind11 code encapsulating a class binding.
        Example:

        py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
          .def(py::init<NodeRef>(),               "frequency"_a = NodeRef(440.0))
          .def(py::init<float>(),                 "frequency"_a = NodeRef(440.0))
          .def(py::init<std::vector<NodeRef>>(),  "frequency"_a = NodeRef(440.0))
          .def(py::init<std::vector<float>>(),    "frequency"_a = NodeRef(440.0));
    """
    if cls.name in omitted_classes:
        return ""
    if cls.docs is None:
        raise ValueError("No docs for class: %s" % cls.name)
    parent_class = cls.parent if cls.parent in known_parent_classes else "Node"

    output = 'py::class_<{class_name}, {superclass}, NodeRefTemplate<{class_name}>>(m, "{class_name}", "{class_docs}")\n'.format(
        class_name=cls.name, class_docs=cls.docs, superclass=parent_class)
    for parameter_set in cls.constructors:
        parameter_type_list = ", ".join([parameter.type for parameter in parameter_set])
        output += '    .def(py::init<{parameter_type_list}>()'.format(parameter_type_list=parameter_type_list);
        for parameter in parameter_set:
            if parameter.default is not None:
                default = parameter.default
                # property defaults
                if default == "{}":
                    default = 0
                elif default == "":
                    default = '""'
                default = "%s" % default
                output += ', "{name}"_a = {default}'.format(name=parameter.name, default=default)
            else:
                output += ', "{name}"_a'.format(name=parameter.name)
        output += ')\n'

    # TODO: This needs refactoring into something general-purpose!
    if cls.name == "VampAnalysis":
        output += '.def("list_plugins", &VampAnalysis::list_plugins, R"pbdoc(list[str]: List the available plugin names.)pbdoc")\n'

    output = output[:-1] + ";\n"
    return output


def extract_docs(doxygen: str) -> str:
    """
    Given a quoted doxygen comment, extract just the documentation text, condensed onto a
    single line.

    Args:
        doxygen: The quoted comment

    Returns:
        The documentation text.
    """
    lines = doxygen.split("\n")
    output = ""
    for line in lines:
        # start or end of comment
        if re.search(r"^\s*/\*", line) or re.search(r"\*/\s*$", line):
            continue
        line = re.sub(r"^\s*\*\s*", "", line)
        output = output + line + " "
    return output.strip()


def sanitize_type(p_type: str) -> str:
    """
    Translate type names into qualified forms that don't rely on typedefs.
    .
    Args:
        p_type: The input type

    Returns:
        The output type.
    """
    if p_type in ["sample", "double"]:
        p_type = "float"
    if p_type in ["string"]:
        p_type = "std::string"
    return p_type


def folder_name_to_title(folder_name: str) -> str:
    """
    Translate a folder name into a title.
    e.g. processing/delays -> "Processing: Delays"
    """
    folder_parts = [part.title() for part in folder_name.split("/")]
    folder_title = ": ".join(folder_parts)
    # capitalise all-vowel or all-consonant folder names (io, fft)
    if re.search(r"^[aeiou]+$", folder_title) or re.search(r"^[^aeiou]+$", folder_title):
        folder_title = folder_title.upper()
    return folder_title


def parse_node_classes(source_files) -> dict[str, list[Parameter]]:
    classes = {}
    classes["io"] = [
        NodeClass("AudioIn", None, [[]], "Audio input"),
        NodeClass("AudioOut_Abstract", None, [], "Abstract audio output"),
        NodeClass("AudioOut_Dummy", "AudioOut_Abstract", [[
            Parameter("num_channels", "int", 2),
            Parameter("buffer_size", "int", "SIGNALFLOW_DEFAULT_BLOCK_SIZE"),
        ]], "Dummy audio output for offline processing"),
        NodeClass("AudioOut", "AudioOut_Abstract", [[
            Parameter("backend_name", "std::string", ""),
            Parameter("device_name", "std::string", ""),
            Parameter("sample_rate", "int", 0),
            Parameter("buffer_size", "int", 0),
        ]], "Audio output")
    ]

    class_category = None
    folder_last = None

    for source_file in source_files:
        folder = re.sub(".*node/", "", source_file)
        folder = os.path.dirname(folder)
        if folder != folder_last:
            folder_last = folder
            class_category = folder
            classes[class_category] = []

        header = CppHeaderParser.CppHeader(source_file)

        for class_name, value in header.classes.items():
            # --------------------------------------------------------------------------------
            # Check whether class is a subclass of a valid Node class
            # --------------------------------------------------------------------------------
            if "inherits" in value and len(value["inherits"]):
                parent_class = value["inherits"][0]["class"]
                if parent_class not in node_superclasses:
                    continue
            else:
                continue

            # --------------------------------------------------------------------------------
            # Parse class documentation and constructors
            # --------------------------------------------------------------------------------
            class_docs = extract_docs(value["doxygen"]) if "doxygen" in value else class_name
            constructor_parameter_sets = []
            for method in value["methods"]["public"]:
                if method["constructor"]:
                    parameters = [
                        Parameter(name=parameter["name"],
                                  type=sanitize_type(parameter["type"]),
                                  default=parameter["defaultValue"] if "defaultValue" in parameter else None)
                        for parameter in method["parameters"]]
                    constructor_parameter_sets.append(parameters)

            # --------------------------------------------------------------------------------
            # If the class has at least one valid constructor, generate output.
            # --------------------------------------------------------------------------------
            if constructor_parameter_sets:
                cls = NodeClass(class_name,
                                parent_class,
                                constructor_parameter_sets,
                                class_docs)
                classes[class_category].append(cls)

    return classes


def generate_bindings(node_classes) -> None:
    """
    Generate the complete set of Python bindings from a dict of all Node classes.

    Args:
        node_classes: dict of all Node classes

    Returns:
        The complete bindings .cpp file, to output to source/src/python/nodes.cpp
    """
    output = ""
    for folder, classes in node_classes.items():
        for cls in classes:
            if cls.name in macos_only_classes:
                output += "#ifdef __APPLE__\n\n"
            if cls.name in vamp_only_classes:
                output += "#ifdef HAVE_VAMP\n\n"

            output += generate_class_bindings(cls)
            output = output.strip()
            output += "\n\n"
            if cls.name in macos_only_classes:
                output += "#endif\n\n"
            if cls.name in vamp_only_classes:
                output += "#endif\n\n"

    output = re.sub("\n", "\n    ", output)
    output = '''#include "signalflow/python/python.h"
    
    void init_python_nodes(py::module &m)
    {{
        /*--------------------------------------------------------------------------------
         * Node subclasses
         *-------------------------------------------------------------------------------*/
        {output}
    }}
    '''.format(output=output)

    with open(bindings_output_path, "w") as fd:
        fd.write(output)
        print("Written Python bindings to %s" % bindings_output_path)


def generate_node_library_index(node_classes) -> str:
    """
    Generate Markdown documentation for the Node reference library

    Args:
        node_classes: dict of all Node classes

    Returns:
        The complete docs/library.md markdown file
    """
    output_markdown = "# Node reference library\n\n"
    output_markdown += "<!-- This code is autogenerated and should not be modified by hand. -->\n\n"

    for folder, classes in node_classes.items():
        if folder in documentation_omit_folders:
            continue
        folder_title = folder_name_to_title(folder)
        output_markdown += "\n## " + folder_title + "\n\n"
        for cls in classes:
            if cls.name in node_superclasses:
                continue
            if cls.name in documentation_omit_classes:
                continue
            if cls.constructors:
                cls_folder = os.path.join(folder, cls.identifier)
                cls_doc_path = os.path.join(cls_folder, "index.md")
                output_markdown += "- **[%s](%s)**: %s\n" % (cls.name, cls_doc_path, cls.docs)
        output_markdown += "\n---\n"

    return output_markdown


def generate_node_library(node_classes):
    """
    Generate Markdown documentation for the Node reference library

    Args:
        node_classes: dict of all Node classes

    Returns:
        The complete docs/library.md markdown file
    """

    # Also outputs mkdocs index with following format:
    #   - Analysis:
    #      - Analysis: library/analysis/index.md
    #      - CrossCorrelate: library/analysis/crosscorrelate.md

    root_directory = "docs/library"

    # --------------------------------------------------------------------------------
    # Generate index
    # --------------------------------------------------------------------------------
    index_file = os.path.join(root_directory, 'index.md')
    index_markdown = generate_node_library_index(node_classes)
    with open(index_file, "w") as fd:
        fd.write(index_markdown)
        print("Written index to %s" % index_file)

    for folder, classes in node_classes.items():
        if folder in documentation_omit_folders:
            continue
        folder_title = folder_name_to_title(folder)
        folder_path = os.path.join(root_directory, folder)
        os.makedirs(folder_path, exist_ok=True)
        folder_index_path_rel = os.path.join(folder, "index.md")
        folder_index_path_abs = os.path.join(root_directory, folder_index_path_rel)
        print(" - \"%s\": library/%s" % (folder_title, folder_index_path_rel))
        with open(folder_index_path_abs, "w") as fd:
            fd.write(f"[Reference library](../index.md) > [{folder_title}](index.md)\n\n")
            fd.write(f"# {folder_title}\n\n")

            for cls in classes:
                if cls.name in node_superclasses:
                    continue
                if cls.name in documentation_omit_classes:
                    continue

                if cls.constructors:
                    fd.write("- **[%s](%s/index.md)**: %s\n" % (cls.name, cls.identifier, cls.docs))

        for cls in classes:
            if cls.name in node_superclasses:
                continue
            if cls.name in documentation_omit_classes:
                continue

            if cls.constructors:
                cls_doc_path = os.path.join(cls.identifier, "index.md")
                cls_doc_abs_path = os.path.join(root_directory, folder, cls_doc_path)
                cls_doc_abs_folder = os.path.dirname(cls_doc_abs_path)
                os.makedirs(cls_doc_abs_folder, exist_ok=True)
                example_scripts = glob.glob(os.path.join(cls_doc_abs_folder, "*.py"))
                example_scripts = list(sorted(example_scripts))
                with open(cls_doc_abs_path, "w") as fd:
                    output_markdown_params = ", ".join(("%s=%s" % (param.name, param.default)) \
                            for param in cls.constructors[0])
                    output_markdown_params = output_markdown_params.replace("nullptr", "None")

                    fd.write(f"title: {cls.name} node documentation\n")
                    fd.write(f"description: {cls.name}: {cls.docs}\n\n")
                    fd.write(
                        f"[Reference library](../../index.md) > [{folder_title}](../index.md) > [{cls.name}](index.md)\n\n")

                    fd.write(f"# {cls.name}\n\n")
                    fd.write(f"```python\n")
                    fd.write(f"{cls.name}({output_markdown_params})\n")
                    fd.write(f"```\n\n")
                    fd.write(f"{cls.docs}\n\n")

                    if len(example_scripts):
                        fd.write(f"### Examples\n\n")
                        for example_script in example_scripts:
                            #--------------------------------------------------------------------------------
                            # For each example script:
                            #  - read the code
                            #  - strip any leading boilerplate (up till the creation of the AudioGraph)
                            #  - strip any trailing boilerplate (graph.wait())
                            #--------------------------------------------------------------------------------
                            example_code = open(example_script, "r").read()
                            example_code = re.sub(r".*graph = AudioGraph\(\)\n", "", example_code, flags=re.DOTALL)
                            example_code = re.sub(r"graph.wait\(\).*", "", example_code, flags=re.DOTALL)
                            fd.write(f"```python\n")
                            fd.write(example_code)
                            fd.write(f"\n```\n\n")

def generate_readme(node_classes) -> None:
    """
    Generate a tabular list of all Node classes, in Markdown format.
    Used for README.md.

    Args:
        node_classes: dict of all Node classes

    Returns:
        The table of node classes, grouped by category
    """
    table = "| Category | Classes  |\n"
    table += "|:---------|:---------|\n"
    for folder, classes in node_classes.items():
        if folder in documentation_omit_folders:
            continue

        category_text = folder_name_to_title(folder)
        class_links = []
        for cls in classes:
            library_url_root = "https://signalflow.dev/library"
            library_url = os.path.join(library_url_root, folder, cls.name.lower() + "/")
            class_link = "[%s](%s)" % (cls.name, library_url)
            class_links.append(class_link)
        table += "| **%s** | %s |\n" % (category_text, ", ".join(class_links))
    table += "\n"

    readme_path = "README.md"
    with open(readme_path, "r") as fd:
        readme_data = fd.read()
    table_re = "\\| Category.*?\n\n"
    readme_data = re.sub(table_re, table, readme_data, flags=re.DOTALL)

    with open(readme_path, "w") as fd:
        fd.write(readme_data)
    print("Written table to %s" % readme_path)


def main(args):
    top_level = subprocess.check_output(["git", "rev-parse", "--show-toplevel"]).decode().strip()
    os.chdir(top_level)
    source_files = get_all_source_files()
    node_classes = parse_node_classes(source_files)

    if not (args.readme or args.library or args.bindings):
        raise ValueError("One of --readme, --bindings or --library must be specified")

    if args.bindings:
        generate_bindings(node_classes)
    if args.readme:
        generate_readme(node_classes)
    if args.library:
        generate_node_library(node_classes)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", "--bindings", help="Generate Python bindings", action="store_true")
    parser.add_argument("-l", "--library", help="Generate node library", action="store_true")
    parser.add_argument("-r", "--readme", help="Generate README", action="store_true")
    parser.add_argument("-a", "--all", help="Generate all", action="store_true")
    args = parser.parse_args()
    if args.all:
        args.bindings = True
        args.library = True
        args.readme = True

    main(args)
