#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace ce {
class ColladaMaterial : public Collada {
  public:
    ColladaMaterial(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
    virtual ~ColladaMaterial() = default;
    void create(Entity& entity, const pugi::xml_node& node);
};
} // namespace ce
