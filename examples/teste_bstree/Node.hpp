#ifndef __BSPTREE_NODE__HPP
#define __BSPTREE_NODE__HPP

#include "ListPolygon.hpp"
#include "Polygon.hpp"

// Ref:
// https://miztakenjoshi.wordpress.com/2009/11/06/hidden-surface-removal-using-binary-space-partition-bsp-algorithm-in-opengl/

enum SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

class Node {
  public:
    Node();

    SIDE classifyPolygon(Polygon* cPolygon);
    float classify(glm::vec3* plane, glm::vec3* eye);

    void BuildTree(Node* rootNode, ListPolygon polygons);
    void DrawTree(Node* finaltree, glm::vec3* eyepoint, ListPolygon* finalfaces);

    Polygon partition;
    ListPolygon polygons;
    Node* frontNode;
    Node* backNode;

    bool v;
};

#endif