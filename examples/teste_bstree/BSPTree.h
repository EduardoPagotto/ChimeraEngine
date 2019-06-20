#ifndef __BSPTREE_HPP
#define __BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "BSPTreeNode.hpp"

class BSPTree {
  public:
    BSPTree(BSPTreeNode* _pRoot) : root(_pRoot) {}
    void draw(glm::vec3* eye, std::vector<Triangle>* _pListPolygon);

  private:
    static void drawBSPTree(BSPTreeNode* tree, glm::vec3* eye, std::vector<Triangle>* _pListPolygon);
    static float classify(glm::vec3* normal, glm::vec3* eye);

    BSPTreeNode* root;
};

#endif
