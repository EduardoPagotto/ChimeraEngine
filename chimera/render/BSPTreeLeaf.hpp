#ifndef __CHIMERA_PVS_HPP
#define __CHIMERA_PVS_HPP

#include "BSPTree.hpp"

namespace Chimera {

class BspTreeLeaf : public BspTreeBase {
  public:
    BspTreeLeaf();
    virtual ~BspTreeLeaf();

  protected:
    BSPTreeNode* bsptreeBuild(std::list<Triangle*>& _vTriangle);
};
} // namespace Chimera

#endif