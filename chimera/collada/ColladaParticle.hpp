#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace ce {
class ColladaParticle : public Collada {
  public:
    ColladaParticle(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
    virtual ~ColladaParticle() = default;
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node nParticle);
};
} // namespace ce