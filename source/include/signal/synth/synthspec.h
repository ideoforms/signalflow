#pragma once

#include "signal/node/node.h"
#include "signal/synth/nodespec.h"

namespace libsignal
{

class SynthSpec
{
public:
    SynthSpec(std::string name);

    /*----------------------------------------------------------------------------------
     * Methods for creating a SynthSpec from NodeSpecs.
     *---------------------------------------------------------------------------------*/
    void add_node_def(NodeSpec def);
    void set_output(NodeSpec def);
    NodeSpec *get_node_def(int id);
    NodeSpec get_root();

    /*----------------------------------------------------------------------------------
     * Save a SynthSpec to disk.
     *---------------------------------------------------------------------------------*/
    void save(std::string filename);

    /*----------------------------------------------------------------------------------
     * Load a SynthSpec from disk.
     *---------------------------------------------------------------------------------*/
    void load(std::string filename);

    /*----------------------------------------------------------------------------------
     * Store a SynthSpec to the global SynthRegistry so that it can be
     * instantiated by name.
     *---------------------------------------------------------------------------------*/
    void store();

    /**----------------------------------------------------------------------------------
     * Print a representation to stdout
     *---------------------------------------------------------------------------------*/
    void print();

    /**----------------------------------------------------------------------------------
     * Returns true if this SynthSpec is ready to be played.
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
class SynthSpecRefTemplate : public std::shared_ptr<T>
{
public:
    SynthSpecRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    SynthSpecRefTemplate(SynthSpec *ptr)
        : std::shared_ptr<T>(ptr) {}
};

typedef SynthSpecRefTemplate<SynthSpec> SynthSpecRef;

}
