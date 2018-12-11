#ifndef __BSPTREE_NODE__HPP
#define __BSPTREE_NODE__HPP

#include "List.hpp"
#include "Polygon.hpp"

enum SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

class Node {
  public:
    Node();
    void BuildTree(Node* rootNode, List polygons);
    SIDE classifyPolygon(Polygon* cPolygon);
    float classify(glm::vec3* plane, glm::vec3* eye);
    void DrawTree(Node* finaltree, glm::vec3* eyepoint, List* finalfaces);

    Polygon partition;
    List polygons;
    Node* frontNode;
    Node* backNode;

    bool v;
};

#endif