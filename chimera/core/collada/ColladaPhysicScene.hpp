#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaPhysicScene : public Collada {
  public:
    ColladaPhysicScene(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaPhysicScene();

    const pugi::xml_node findModel(pugi::xml_node node, const std::string& body);
    void loadAll(pugi::xml_node node, Registry* reg);

  private:
};

} // namespace Chimera