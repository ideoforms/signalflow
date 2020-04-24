#pragma once

#include "signal/node/node.h"
#include "signal/synth/nodedef.h"

namespace libsignal
{

class SynthSpec
{
public:
    SynthSpec(std::string name);

    /*----------------------------------------------------------------------------------
     * Methods for creating a SynthSpec from NodeDefinitions.
     *---------------------------------------------------------------------------------*/
    void add_node_def(NodeDefinition def);
    void set_output(NodeDefinition def);
    NodeDefinition *get_node_def(int id);
    NodeDefinition get_root();

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
     * Returns true if this SynthSpec is ready to be played.
     *---------------------------------------------------------------------------------*/
    bool is_ready();

    std::string name;

    // private:

    NodeDefinition output_def;
    bool parsed = false;
    int last_id = 0;

    std::unordered_map<int, NodeDefinition> nodedefs;
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
