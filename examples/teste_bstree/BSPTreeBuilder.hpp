#ifndef __BSPTREE_BUILDER_HPP
#define __BSPTREE_BUILDER_HPP

#include "BSPTreeNode.hpp"
#include <glm/glm.hpp>

class BSPTreeBuilder {
  public:
    BSPTreeBuilder(ListPolygon* polygons);
    BSPTreeNode* getNodeRoot() { return root; }

  private:
    static float f(const glm::vec3& p, Polygon* _pPartition);

    static BSPTreeNode* buildBSPTreeNode(ListPolygon polygons);
    static SIDE classifyPolygon(Polygon* _pPartition, Polygon* _pPolygon);
    static void splitPolygon(Polygon* _poly, Polygon* _partition, ListPolygon* _polygons);

    BSPTreeNode* root;
};

template <class T> void swap(T& a, T& b);

glm::vec3 intersect(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& a, const glm::vec3& c);

#endif