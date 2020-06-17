#ifndef __CHIMERA_PVS_HPP
#define __CHIMERA_PVS_HPP

#include "Plane.hpp"
#include "Triangle.hpp"
#include "VertexData.hpp"
#include <vector>

namespace Chimera {

struct Node {
    unsigned char IsLeaf;
    unsigned long Plane;
    unsigned long Front;
    signed long Back;
    // BOUNDINGBOX BoundingBox;
};

// class Tris {
//   public:
//     Tris(Triangle* t) {
//         triangle = t;
//         normal = triangle->getNormal();
//         beenUsedAsSplitter = false;
//     };

//     Triangle* triangle;
//     glm::vec3 normal;
//     bool beenUsedAsSplitter;
//     long textureIndex;
// };

class PVS {
  public:
    PVS() {}
    unsigned int selectBestSplitter(std::vector<Triangle*>& _poliyList);

  private:
    inline glm::vec3 vPosVal(Triangle* _t, const unsigned& pos) { return vVertex[_t->p[pos]].position; }

    std::vector<Plane*> planes;
    std::vector<Chimera::VertexData> vVertex;
};

} // namespace Chimera

#endif