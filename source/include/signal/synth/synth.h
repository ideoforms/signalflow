#pragma once

#include "signal/synth/nodedef.h"
#include "signal/synth/synthspec.h"
#include "signal/synth/synthtemplate.h"

namespace libsignal
{
class Synth
{
public:
    Synth();
    Synth(SynthSpecRef synthspec);
    Synth(SynthTemplateRef synthtemplate);
    Synth(std::string name);

    void set_input(std::string name, float value);
    void set_input(std::string name, NodeRef value);
    void disconnect();
    bool get_auto_free();
    void set_auto_free(bool value);
    void node_state_changed(NodeRef node);

    NodeRef output;
    std::unordered_map<std::string, NodeRef> inputs;
    std::set<NodeRef> nodes;

private:
    NodeRef instantiate(NodeDefinition *nodedef);
    bool auto_free;
};


template <class T>
class SynthRefTemplate : public std::shared_ptr<T>
{
public:
    SynthRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    SynthRefTemplate(Synth *ptr)
        : std::shared_ptr<T>(ptr) {}
    SynthRefTemplate(SynthSpecRef synthspec)
        : std::shared_ptr<T>(new T(synthspec)) {}
};

typedef SynthRefTemplate<Synth> SynthRef;


}
