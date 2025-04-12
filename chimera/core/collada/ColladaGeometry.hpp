#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace ce {
class ColladaGeometry : public Collada {
  public:
    ColladaGeometry(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc) : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaGeometry() {}
    void create(Entity& entity, pugi::xml_node geo);

  private:
};

} // namespace ce