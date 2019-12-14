#ifndef __CHIMERA_LOADER_LIST_NODES__HPP
#define __CHIMERA_LOADER_LIST_NODES__HPP

#include <vector>

#include "chimera/core/Singleton.hpp"
#include "chimera/node/Node.hpp"
#include "chimera/node/NodeMesh.hpp"

namespace ChimeraLoaders {

class ListNodes {
    friend class Chimera::Singleton<ListNodes>;

  public:
    void reset() noexcept {
        mapMesh.clear();
        mapMeshNode.clear();
    }

    std::map<std::string, Chimera::NodeMesh*> mapMesh;
    std::map<std::string, Chimera::Node*> mapMeshNode;

  protected:
    ListNodes(){};
    virtual ~ListNodes() { reset(); }
    ListNodes(const ListNodes&) = delete;
};
} // namespace ChimeraLoaders
#endif