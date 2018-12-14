#ifndef __BSPTREE_BASE__HPP
#define __BSPTREE_BASE__HPP

#include "ListPolygon.hpp"
#include "Polygon.hpp"

#include <vector>

// Ref:
// https://miztakenjoshi.wordpress.com/2009/11/06/hidden-surface-removal-using-binary-space-partition-bsp-algorithm-in-opengl/

enum SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

struct HyperPlane {
    std::vector<float> coefficients;
};

struct BSPTreeNode {
    ListPolygon polygons;
    Polygon partition; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
};

struct BSPTree {
    BSPTreeNode* root;
};

BSPTreeNode* buildBSPTreeNode(ListPolygon polygons);
BSPTree* buildBSPTree(ListPolygon* polygons);
SIDE classifyPolygon(Polygon* _pPartition, Polygon* _pPolygon);
float classify(glm::vec3* normal, glm::vec3* eye);
void drawBSPTree(BSPTreeNode* tree, glm::vec3* eye, ListPolygon* finalFaces);
void splitPolygon(Polygon* _poly, Polygon* _partition, Polygon* _front_piece, Polygon* _back_piece);

#endif
