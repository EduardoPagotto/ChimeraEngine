#pragma once
#include "chimera/core/Singleton.hpp"
#include "chimera/node/Node.hpp"
#include "chimera/node/NodeMesh.hpp"
#include <vector>

namespace Chimera {

class ListNodes {
    friend class Singleton<ListNodes>;

  public:
    void reset() noexcept {
        mapMesh.clear();
        mapMeshNode.clear();
    }

    std::map<std::string, NodeMesh*> mapMesh;
    std::map<std::string, Node*> mapMeshNode;

  protected:
    ListNodes(){};
    virtual ~ListNodes() { reset(); }
    ListNodes(const ListNodes&) = delete;
};
} // namespace Chimera
