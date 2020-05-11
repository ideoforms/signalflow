#pragma once

#include "signal/patch/patchspec.h"
#include <unordered_map>

namespace libsignal
{
class PatchRegistry
{
public:
    PatchRegistry();
    static PatchRegistry *global();

    Patch *create(std::string name);
    void add(std::string name, PatchSpecRef patchspec);
    PatchSpecRef get(std::string name);

    std::unordered_map<std::string, PatchSpecRef> patchspecs;
};
}
