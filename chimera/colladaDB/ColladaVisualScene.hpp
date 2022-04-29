#pragma once
#include "chimera/colladaDB/Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaVisualScene : public Collada {
  public:
    ColladaVisualScene(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaVisualScene();

    void loadNode(pugi::xml_node node, Registry* reg);
    void loadAll(pugi::xml_node node, Registry* reg);

  private:
};

} // namespace Chimera