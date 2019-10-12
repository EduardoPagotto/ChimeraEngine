#ifndef __BSPTREE_BUILDER_HPP
#define __BSPTREE_BUILDER_HPP

#include "BSPTreeNode.hpp"
#include <glm/glm.hpp>

class BSPTreeBuilder {
  public:
    BSPTreeBuilder(std::vector<Triangle>* _pListPolygon);
    BSPTreeNode* getNodeRoot() { return root; }

  private:
    static float f(const glm::vec3& p, Triangle* _pPartition);

    static BSPTreeNode* buildBSPTreeNode(std::vector<Triangle>* _pListPolygon);
    static void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Triangle* _partition,
                              std::vector<Triangle>* _pListPolygon);

    static SIDE classifyPoly(Triangle* plane, Triangle* poly, glm::vec3& _resultTest);

    BSPTreeNode* root;
};

template <class T> void swapFace(T& a, T& b);
glm::vec3 aprox(const glm::vec3& dado);
glm::vec3 intersect(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& a, const glm::vec3& c);

#endif