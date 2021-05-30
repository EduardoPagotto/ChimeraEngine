#ifndef __CHIMERA_BSPTREE_NODE_HPP
#define __CHIMERA_BSPTREE_NODE_HPP

#include "Leaf.hpp"
#include "chimera/render/Plane.hpp"
#include "chimera/render/Triangle.hpp"

namespace Chimera {

class BSPTreeNode {
  public:
    BSPTreeNode(const Plane& _hyperPlane);
    virtual ~BSPTreeNode();
    void destroy();
    void addPolygon(Triangle* _triangle);
    void addIndexPolygon(std::list<Triangle*>& _vTriangle);

    Leaf* pLeaf;
    Plane hyperPlane; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
    bool isSolid;
    bool isLeaf;
};

} // namespace Chimera
#endif