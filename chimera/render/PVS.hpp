#ifndef __CHIMERA_PVS_HPP
#define __CHIMERA_PVS_HPP

#include "Plane.hpp"
#include "Triangle.hpp"
#include <vector>

namespace Chimera {

struct Node {
    unsigned char IsLeaf;
    unsigned long Plane;
    unsigned long Front;
    signed long Back;
    // BOUNDINGBOX BoundingBox;
};

class Tris {
  public:
    Tris(Triangle* t) {
        triangle = t;
        normal = triangle->normal();
        beenUsedAsSplitter = false;
    };

    Triangle* triangle;
    glm::vec3 normal;
    bool beenUsedAsSplitter;
    long textureIndex;
};

class PVS {
  public:
    PVS() {}
    unsigned int selectBestSplitter(std::vector<Tris>& _poliyList);

    std::vector<Plane*> planes;
};

} // namespace Chimera

#endif