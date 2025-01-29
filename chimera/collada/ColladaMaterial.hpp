#pragma once
#include "Collada.hpp"
#include "core/Registry.hpp"

namespace ce {
class ColladaMaterial : public Collada {
  public:
    ColladaMaterial(ColladaDom& dom, const std::string& url) : Collada(dom, url) {};
    virtual ~ColladaMaterial() {}
    void create(Entity& entity, const pugi::xml_node& node);
};
} // namespace ce
