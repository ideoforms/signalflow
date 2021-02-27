#pragma once

#include <functional>
#include <unordered_map>

#include "signalflow/patch/patch-node-spec.h"

#define REGISTER(CLASS, NAME) static bool CLASS##OK = NodeRegistry::global()->add<CLASS>(NAME);

namespace signalflow
{

class Node;
class Patch;

template <typename T>
Node *create()
{
    return new T;
}

class NodeRegistry
{
    /**------------------------------------------------------------------------
 * The NodeRegistry is a singleton that stores all of the known Node
 * classes by name. Each Node class header calls the REGISTRY macro
 * to register the class, so that objects of the class can be
 * constructed programmatically by using the class name:
 *
 *   NodeRef player = NodeRegistry::global()->create("buffer-player")
 *
 * This is needed when a graph is constructed from json or other
 * serialisation.
 *
 *-----------------------------------------------------------------------*/

public:
    NodeRegistry();
    virtual ~NodeRegistry();

    /**------------------------------------------------------------------------
     * Return the global node registry.
     *
     *-----------------------------------------------------------------------*/
    static NodeRegistry *global();

    /**------------------------------------------------------------------------
     * Instantiate a Node of the given name.
     * @param name The name given when registering the class with ->add
     *
     *-----------------------------------------------------------------------*/
    Node *create(std::string name);

    /*------------------------------------------------------------------------
     * (Function template implementations must be in .h file.)
     * http://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
     *-----------------------------------------------------------------------*/
    template <class T>
    bool add(std::string name)
    {
        classes[name] = &::signalflow::create<T>;
        return true;
    }

private:
    /*------------------------------------------------------------------------
     * Maps node names to constructors
     *-----------------------------------------------------------------------*/
    std::unordered_map<std::string, std::function<Node *()>> classes;
};

}
