#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace ce {
class ColladaLight : public Collada {
  public:
    ColladaLight(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc) : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaLight() {}
    void create(Entity& entity, pugi::xml_node nodeLight);

  private:
};

} // namespace ce