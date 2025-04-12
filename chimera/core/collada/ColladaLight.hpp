#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace ce {
class ColladaLight : public Collada {
  public:
    ColladaLight(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
    virtual ~ColladaLight() = default;
    void create(Entity& entity, pugi::xml_node nodeLight);
};

} // namespace ce