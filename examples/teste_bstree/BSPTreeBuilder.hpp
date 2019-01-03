#ifndef __BSPTREE_BUILDER_HPP
#define __BSPTREE_BUILDER_HPP

#include "BSPTreeNode.hpp"
#include <glm/glm.hpp>

class BSPTreeBuilder {
  public:
    BSPTreeBuilder(ListPolygon* polygons);
    BSPTreeNode* getNodeRoot() { return root; }

  private:
    static float f(const glm::vec3& p, Triangle* _pPartition);

    static BSPTreeNode* buildBSPTreeNode(ListPolygon polygons);
    static SIDE classifyPolygon(Triangle* _pPartition, Triangle* _pPolygon);
    static void splitPolygon(Triangle* _poly, Triangle* _partition, ListPolygon* _polygons);

    BSPTreeNode* root;
};

template <class T> void swap(T& a, T& b);

glm::vec3 intersect(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& a, const glm::vec3& c);

#endif