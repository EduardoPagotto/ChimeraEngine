#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace ce {
class ColladaVisualScene : public Collada {
  public:
    ColladaVisualScene(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc)
        : Collada(dom, url, serviceLoc) {}

    virtual ~ColladaVisualScene() {}

    void loadNode(pugi::xml_node node);
    void loadAll(pugi::xml_node node);

  private:
    void nodeData(pugi::xml_node node, Entity entity);
};

} // namespace ce