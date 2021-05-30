#include "chimera/render/bsp/BSPTreeNode.hpp"

namespace Chimera {

BSPTreeNode::BSPTreeNode(const Plane& _hyperPlane)
    : hyperPlane(_hyperPlane), front(nullptr), back(nullptr), isLeaf(false), pLeaf(nullptr), isSolid(false) {}

BSPTreeNode::~BSPTreeNode() { this->destroy(); }

void BSPTreeNode::addPolygon(Triangle* _triangle) {

    if (pLeaf == nullptr)
        pLeaf = new Leaf;

    pLeaf->addFace(_triangle->getSerial(), _triangle->p[0], _triangle->p[1], _triangle->p[2]);

    delete _triangle;
    _triangle = nullptr;

} // namespace Chimera

void BSPTreeNode::addIndexPolygon(std::list<Triangle*>& _vTriangle) {

    if (pLeaf == nullptr)
        pLeaf = new Leaf;

    while (_vTriangle.empty() == false) {
        Triangle* convPoly = _vTriangle.back();
        _vTriangle.pop_back();
        pLeaf->addFace(convPoly->getSerial(), convPoly->p[0], convPoly->p[1], convPoly->p[2]);

        delete convPoly;
        convPoly = nullptr;
    }
}

void BSPTreeNode::destroy() {

    if (pLeaf != nullptr) {
        delete pLeaf;
        pLeaf = nullptr;
    }
}
} // namespace Chimera