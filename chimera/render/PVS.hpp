#ifndef __CHIMERA_PVS_HPP
#define __CHIMERA_PVS_HPP

#include "AABB.hpp"
#include "Plane.hpp"
#include "Triangle.hpp"
#include "VertexData.hpp"
#include <vector>

namespace Chimera {

struct NodePVS { // as NODE
    unsigned char IsLeaf;
    uint32_t plane;   // Plane; // as inidice!!!!
    uint32_t front;   // Front; // NodePVS* Front; (index)
    int32_t back;     // Back;   // NodePVS* Back;
    AABB boundingBox; // BOUNDINGBOX BoundingBox;
};

struct LEAF {                 // each leaf to have a maximum of 50 portals which is way to many but lets not
    long StartPolygon;        // take any chances.This memory is only wasteful during PVS Computing and would
    long EndPolygon;          // not be saved to a bsp file for runtime rendering.(Unless you wanted the
    long PortalIndexList[50]; // portal information)
    long NumberOfPortals;
    long PVSIndex; // Because the PVS is compressed this if the offset into the PVSData
                   // array that this leafs PVS Set begins
    AABB BoundingBox;
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
    PVS();
    unsigned int selectBestSplitter(std::vector<Triangle*>& _poliyList);

  private:
    void buildBspTree(long Node, std::vector<Triangle*>& PolyList);

    inline glm::vec3 vPosVal(Triangle* _t, const unsigned& pos) { return vVertex[_t->p[pos]].position; }

    void IncreaseNumberOfPolygons() { NumberOfPolygons++; }

    std::vector<NodePVS> nodeArray;
    std::vector<Plane> planeArray;

    std::vector<LEAF> LeafArray;

    std::vector<Plane*> planes;
    std::vector<Chimera::VertexData> vVertex;

    long NumberOfPolygons;
    long NumberOfNodes;
    long NumberOfLeafs;
    long NumberOfPlanes;
    long NumberOfPortals;
};

} // namespace Chimera

#endif