#ifndef __CHIMERA_PVS_HPP
#define __CHIMERA_PVS_HPP

#include "BSPTree.hpp"

namespace Chimera {

class PVS : public BspTreeBase {
  public:
    PVS();
    virtual ~PVS();

  protected:
    BSPTreeNode* bsptreeBuild(std::list<Triangle*>& _vTriangle);
};
} // namespace Chimera

#endif