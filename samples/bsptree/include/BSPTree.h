#ifndef __BSPTREE_HPP
#define __BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "chimera/core/Triangle.hpp"
#include <vector>

enum class SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

// struct HyperPlane {
//     std::vector<float> coefficients;
// };

struct BSPTreeNode {
    BSPTreeNode(const Chimera::Triangle& _partition) : partition(_partition), front(nullptr), back(nullptr) {}
    std::vector<Chimera::Triangle> polygons;
    Chimera::Triangle partition; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
};

class BSPTreeBuilder {
  public:
    BSPTreeBuilder(std::vector<Chimera::Triangle>* _pListPolygon);
    BSPTreeNode* getNodeRoot() { return root; }

  private:
    static float f(const glm::vec3& p, Chimera::Triangle* _pPartition);

    static BSPTreeNode* buildBSPTreeNode(std::vector<Chimera::Triangle>* _pListPolygon);
    static void splitTriangle(const glm::vec3& fx, Chimera::Triangle* _pTriangle, Chimera::Triangle* _partition,
                              std::vector<Chimera::Triangle>* _pListPolygon);

    static SIDE classifyPoly(Chimera::Triangle* plane, Chimera::Triangle* poly, glm::vec3& _resultTest);

    BSPTreeNode* root;
};

template <class T> void swapFace(T& a, T& b);
glm::vec3 aprox(const glm::vec3& dado);
glm::vec3 intersect(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& a, const glm::vec3& c);

//----- PARSER TO DRAW
void drawBSPTree(BSPTreeNode* _pRoot, glm::vec3* eye, std::vector<Chimera::VertexDataFull>* _pOutVertex);

#endif
