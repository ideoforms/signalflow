#pragma once

#include "signal/node/node.h"
#include "signal/patch/nodespec.h"

namespace libsignal
{

class PatchSpec
{
public:
    PatchSpec(std::string name);

    /*----------------------------------------------------------------------------------
     * Methods for creating a PatchSpec from NodeSpecs.
     *---------------------------------------------------------------------------------*/
    void add_node_def(NodeSpec def);
    void set_output(NodeSpec def);
    NodeSpec *get_node_def(int id);
    NodeSpec get_root();

    /*----------------------------------------------------------------------------------
     * Save a PatchSpec to disk.
     *---------------------------------------------------------------------------------*/
    void save(std::string filename);

    /*----------------------------------------------------------------------------------
     * Load a PatchSpec from disk.
     *---------------------------------------------------------------------------------*/
    void load(std::string filename);

    /*----------------------------------------------------------------------------------
     * Store a PatchSpec to the global PatchRegistry so that it can be
     * instantiated by name.
     *---------------------------------------------------------------------------------*/
    void store();

    /**----------------------------------------------------------------------------------
     * Print a representation to stdout
     *---------------------------------------------------------------------------------*/
    void print();

    /**----------------------------------------------------------------------------------
     * Returns true if this PatchSpec is ready to be played.
     *---------------------------------------------------------------------------------*/
    bool is_ready();

    std::string name;

    // private:

    void print(NodeSpec *root, int depth);

    NodeSpec output_def;
    bool parsed = false;
    int last_id = 0;

    std::unordered_map<int, NodeSpec> nodespecs;
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
