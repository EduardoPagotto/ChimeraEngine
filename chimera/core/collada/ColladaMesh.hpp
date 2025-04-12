#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace ce {
class ColladaMesh : public Collada {
  public:
    ColladaMesh(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc) : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaMesh() {}
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node nMesh);

  private:
};
} // namespace ce