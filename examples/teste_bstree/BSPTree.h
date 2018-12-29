#ifndef __BSPTREE_HPP
#define __BSPTREE_HPP

// Ref:
// https://miztakenjoshi.wordpress.com/2009/11/06/hidden-surface-removal-using-binary-space-partition-bsp-algorithm-in-opengl/

#include "BSPTreeNode.hpp"

class BSPTree {
  public:
    BSPTree(BSPTreeNode* _pRoot) : root(_pRoot) {}
    void draw(glm::vec3* eye, ListPolygon* finalFaces);

  private:
    static void drawBSPTree(BSPTreeNode* tree, glm::vec3* eye, ListPolygon* finalFaces);
    static float classify(glm::vec3* normal, glm::vec3* eye);

    BSPTreeNode* root;
};

#endif
