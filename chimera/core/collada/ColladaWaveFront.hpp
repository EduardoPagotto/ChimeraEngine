#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace ce {
class ColladaWaveFront : public Collada {
  public:
    ColladaWaveFront(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
    virtual ~ColladaWaveFront() = default;
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);
};
} // namespace ce