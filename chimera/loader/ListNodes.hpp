#pragma once
#include "chimera/core/Singleton.hpp"
//#include "chimera/node/Node.hpp"
//#include "chimera/node/NodeMesh.hpp"
#include <vector>

namespace Chimera {

class ListNodes {
    friend class Singleton<ListNodes>;

  public:
    void reset() noexcept {}

  protected:
    ListNodes(){};
    virtual ~ListNodes() { reset(); }
    ListNodes(const ListNodes&) = delete;
};
} // namespace Chimera
