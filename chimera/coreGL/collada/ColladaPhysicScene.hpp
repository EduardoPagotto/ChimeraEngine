#pragma once
#include "Collada.hpp"

namespace Chimera {
class ColladaPhysicScene : public Collada {
  public:
    ColladaPhysicScene(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc)
        : Collada(dom, url, serviceLoc) {}
    virtual ~ColladaPhysicScene() {}
    const pugi::xml_node findModel(pugi::xml_node node, const std::string& body);
    void loadAll(pugi::xml_node node);
};
} // namespace Chimera