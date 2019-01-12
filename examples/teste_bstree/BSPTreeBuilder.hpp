#ifndef __BSPTREE_BUILDER_HPP
#define __BSPTREE_BUILDER_HPP

#include "BSPTreeNode.hpp"
#include <glm/glm.hpp>

class BSPTreeBuilder {
  public:
    BSPTreeBuilder(std::vector<Triangle>* _pArrayTriangle);
    BSPTreeNode* getNodeRoot() { return root; }

  private:
    static float f(const glm::vec3& p, Triangle* _pPartition);

    static BSPTreeNode* buildBSPTreeNode(std::vector<Triangle>* _arrayTriangle);
    static glm::vec3 classifyPolygon(Triangle* _pPartition, Triangle* _pTriangle);
    static void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Triangle* _partition,
                              std::vector<Triangle>* _pArrayTriangle);

    static SIDE ClassifyPolyTest(Triangle* Plane, Triangle* Poly);

    BSPTreeNode* root;
};

template <class T> void swapFace(T& a, T& b);

glm::vec3 aprox(const glm::vec3& dado);
glm::vec3 intersect(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& a, const glm::vec3& c);

#endif