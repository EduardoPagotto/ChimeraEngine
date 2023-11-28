#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/Singleton.hpp"

namespace Chimera {
class ColladaPhysicScene : public Collada {
  public:
    ColladaPhysicScene(ColladaDom& dom, const std::string& url) : Collada(dom, url) { r = Singleton<Registry>::get(); }
    virtual ~ColladaPhysicScene() { Singleton<Registry>::release(); }
    const pugi::xml_node findModel(pugi::xml_node node, const std::string& body);
    void loadAll(pugi::xml_node node);

  private:
    Registry* r = nullptr;
};
} // namespace Chimera