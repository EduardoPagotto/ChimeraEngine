#pragma once
#include "chimera/colladaDB/Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaNode : public Collada {
  public:
    ColladaNode() : Collada(){};
    virtual ~ColladaNode();

    void loadNode(pugi::xml_node node, Registry* reg);
    void loadAll(pugi::xml_node node, Registry* reg);

  private:
};

} // namespace Chimera