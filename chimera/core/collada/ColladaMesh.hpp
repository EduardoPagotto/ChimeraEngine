#pragma once
#include "../Registry.hpp"
#include "Collada.hpp"

namespace Chimera {
class ColladaMesh : public Collada {
  public:
    ColladaMesh(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaMesh();
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node nMesh);

  private:
};
} // namespace Chimera