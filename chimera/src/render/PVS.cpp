#include "chimera/render/PVS.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

PVS::PVS() : BspTreeBase() {}

PVS::~PVS() {}

BSPTreeNode* PVS::bsptreeBuild(std::list<Triangle*>& _vTriangle) {

    if (_vTriangle.empty() == true)
        return nullptr;

    Plane partition = selectBestSplitter(_vTriangle);
    BSPTreeNode* tree = new BSPTreeNode(partition);

    std::list<Triangle*> front_list;
    std::list<Triangle*> back_list;

    Triangle* poly = nullptr;

    while (_vTriangle.empty() == false) {

        poly = _vTriangle.back();
        _vTriangle.pop_back();
        glm::vec3 result;
        SIDE clipTest = tree->hyperPlane.classifyPoly(vPosVal(poly, 0), // PA old poly.vertex[0].position
                                                      vPosVal(poly, 1), // PB
                                                      vPosVal(poly, 2), // PC
                                                      &result);         // Clip Test Result (A,B,C)
        switch (clipTest) {
            case SIDE::CP_BACK:
                back_list.push_front(poly);
                break;
            case SIDE::CP_FRONT:
                front_list.push_front(poly);
                break;
            case SIDE::CP_ONPLANE: {
                if (partition.collinearNormal(poly->getNormal()) == true)
                    front_list.push_front(poly);
                else
                    back_list.push_front(poly);
            } break;
            default:
                splitTriangle(result, poly, tree->hyperPlane, _vTriangle);
                break;
        }
    }

    unsigned int count = 0;
    for (auto th : front_list) {
        if (th->beenUsedAsSplitter == false)
            count++;
        // break; //TODO: testar se mais eficiente a contar todos
    }

    if (count == 0) {
        // BSPTreeNode* convex = new BSPTreeNode(partition);
        // convex->isLeaf = true;
        // convex->isSolid = false;
        tree->addIndexPolygon(front_list);
        tree->isSolid = false;
        tree->isLeaf = true;
    } else {
        tree->front = bsptreeBuild(front_list);
    }

    tree->back = bsptreeBuild(back_list);
    if (tree->back == nullptr) {
        if (tree->pLeaf == nullptr) {
            BSPTreeNode* solid = new BSPTreeNode(partition);
            solid->isSolid = true;
            tree->isLeaf = false;
            tree->back = solid;
        }
    }

    return tree;
}
} // namespace Chimera