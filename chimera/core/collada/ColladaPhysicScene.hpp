#pragma once
#include "Collada.hpp"

namespace ce {
class ColladaPhysicScene : public Collada {
  public:
    ColladaPhysicScene(ColladaDom& dom, const std::string& url) : Collada(dom, url) {};
    virtual ~ColladaPhysicScene() = default;
    const pugi::xml_node findModel(pugi::xml_node node, const std::string& body);
    void loadAll(pugi::xml_node node);
};
} // namespace ce