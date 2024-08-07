#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaHeightMap : public Collada {
  public:
    ColladaHeightMap(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc)
        : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaHeightMap() {}
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);

  private:
};
} // namespace Chimera