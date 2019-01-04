#ifndef __BSPTREE_BUILDER_HPP
#define __BSPTREE_BUILDER_HPP

#include "BSPTreeNode.hpp"
#include <glm/glm.hpp>

class BSPTreeBuilder {
  public:
    BSPTreeBuilder(ArrayTriangle* _arrayTriangle);
    BSPTreeNode* getNodeRoot() { return root; }

  private:
    static float f(const glm::vec3& p, Triangle* _pPartition);

    static BSPTreeNode* buildBSPTreeNode(ArrayTriangle _arrayTriangle);
    static glm::ivec3 classifyPolygon(Triangle* _pPartition, Triangle* _pTriangle);
    static void splitTriangle(const glm::ivec3& fx, Triangle* _pTriangle, Triangle* _partition,
                              ArrayTriangle* _pArrayTriangle);

    BSPTreeNode* root;
};

template <class T> void swap(T& a, T& b);

glm::vec3 intersect(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& a, const glm::vec3& c);

#endif