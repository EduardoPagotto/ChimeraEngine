#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/Singleton.hpp"

namespace Chimera {
class ColladaVisualScene : public Collada {
  public:
    ColladaVisualScene(ColladaDom& dom, const std::string& url) : Collada(dom, url) { r = Singleton<Registry>::get(); }
    virtual ~ColladaVisualScene() { Singleton<Registry>::release(); }

    void loadNode(pugi::xml_node node);
    void loadAll(pugi::xml_node node);

  private:
    void nodeData(pugi::xml_node node, Entity entity);
    Registry* r = nullptr;
};

} // namespace Chimera