#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaParticle : public Collada {
  public:
    ColladaParticle(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc) : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaParticle() {}
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node nParticle);

  private:
};
} // namespace Chimera