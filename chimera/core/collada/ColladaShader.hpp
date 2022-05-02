#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaShader : public Collada {
  public:
    ColladaShader(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaShader();
    // void create(Entity& entity, pugi::xml_node nodeParent);
    void create(const std::string& refName, Entity& entity, pugi::xml_node node);

  private:
    // void createEffect(Material* pMat, pugi::xml_node nodeParent);
    // Texture* loadImage(pugi::xml_node nodeParent, const std::string& url);
};
} // namespace Chimera
