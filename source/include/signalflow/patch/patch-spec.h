#pragma once

/**----------------------------------------------------------------------------------
 * @file patch-spec.h
 * @brief PatchSpec describes the structure of a Patch.
 *
 *---------------------------------------------------------------------------------*/

#include "signalflow/node/node.h"
#include "signalflow/patch/patch-node-spec.h"
#include <map>

namespace signalflow
{

class PatchSpec
{
public:
    /**----------------------------------------------------------------------------------
     * Create a new empty PatchSpec.
     *
     *---------------------------------------------------------------------------------*/
    PatchSpec();

    /**----------------------------------------------------------------------------------
     * Load a PatchSpec from the given filename.
     *
     * @param filename Path to a .json file containing the patch specification.
     *---------------------------------------------------------------------------------*/
    PatchSpec(std::string filename);

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods for creating a PatchSpec from NodeSpecs.
    ////////////////////////////////////////////////////////////////////////////////////

    /**----------------------------------------------------------------------------------
     * Add a node spec to this patch spec.
     *
     * @param node The PatchNodeSpec to add
     *---------------------------------------------------------------------------------*/
    void add_node_spec(PatchNodeSpec *node);

    /**----------------------------------------------------------------------------------
     * Define a node spec as this patch spec's output.
     *
     * @param node The PatchNodeSpec to set as output
     *---------------------------------------------------------------------------------*/
    void set_output(PatchNodeSpec *node);

    PatchNodeSpec *get_node_spec(int id);
    PatchNodeSpec *get_output();

    /**----------------------------------------------------------------------------------
     * Query a PatchSpec's name.
     *
     * @returns The human-readable name.
     *---------------------------------------------------------------------------------*/
    std::string get_name();

    /**----------------------------------------------------------------------------------
     * Set a PatchSpec's name.
     *
     * @param name The human-readable name.
     *---------------------------------------------------------------------------------*/
    void set_name(std::string name);

    /**----------------------------------------------------------------------------------
     * Load a PatchSpec from file.
     *
     * @param filename The json file containing the PatchSpec.
     *---------------------------------------------------------------------------------*/
    void load(std::string filename);

    /**----------------------------------------------------------------------------------
     * Save a PatchSpec to file.
     *
     * @param filename The json file to write to.
     *---------------------------------------------------------------------------------*/
    void save(std::string filename);

    /**----------------------------------------------------------------------------------
     * Read a PatchSpec from a json string.
     *---------------------------------------------------------------------------------*/
    void from_json(std::string json);

    /**----------------------------------------------------------------------------------
     * Export a PatchSpec to a json string.
     *
     * @returns A valid json string encapsulating the PatchSpec.
     *---------------------------------------------------------------------------------*/
    std::string to_json();

    /**----------------------------------------------------------------------------------
     * Store a PatchSpec to the global PatchRegistry so that it can be
     * instantiated by name.
     *---------------------------------------------------------------------------------*/
    void store();

    /**----------------------------------------------------------------------------------
     * Print a representation to stdout
     *---------------------------------------------------------------------------------*/
    void print();

protected:
    friend class Patch;

    PatchNodeSpec *output;
    std::map<int, PatchNodeSpec *> nodespecs;

private:
    std::string name;

    void print(PatchNodeSpec *root, int depth);
};

template <class T>
class PatchSpecRefTemplate : public std::shared_ptr<T>
{
public:
    PatchSpecRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    PatchSpecRefTemplate(PatchSpec *ptr)
        : std::shared_ptr<T>(ptr) {}
};

typedef PatchSpecRefTemplate<PatchSpec> PatchSpecRef;

}
