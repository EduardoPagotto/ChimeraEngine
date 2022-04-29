#pragma once
#include "chimera/colladaDB/Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/Material.hpp"

namespace Chimera {
class ColladaMaterial : public Collada {
  public:
    ColladaMaterial(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaMaterial();
    void create(Entity& entity, pugi::xml_node nodeParent);

  private:
    void createShader(const std::string& refName, Entity& entity, pugi::xml_node nodeParent);
    void createEffect(Material* pMat, pugi::xml_node nodeParent);
    Texture* loadImage(pugi::xml_node nodeParent, const std::string& url);
};
} // namespace Chimera
