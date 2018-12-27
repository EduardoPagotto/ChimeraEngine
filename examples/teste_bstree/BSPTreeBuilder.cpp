#include "BSPTreeBuilder.hpp"

BSPTreeBuilder::BSPTreeBuilder(ListPolygon* polygons) { root = buildBSPTreeNode(*polygons); }

void BSPTreeBuilder::splitPolygon(Polygon* _poly, Polygon* _partition, Polygon* _front_piece, Polygon* _back_piece) {}

SIDE BSPTreeBuilder::classifyPolygon(Polygon* _pPartition, Polygon* _pPolygon) {

    glm::vec3 n1 = _pPartition->getFaceNormal();
    glm::vec3 v1 = _pPartition->getVertices()[0];
    float dotVal1 = -(glm::dot(n1, v1));

    float p[3];
    for (int i = 0; i < 3; i++)
        p[i] = glm::dot(n1, _pPolygon->getVertices()[i]) + dotVal1;

    if (p[0] > 0 && p[1] > 0 && p[2] > 0)
        return IS_INFRONT;
    else if (p[0] == 0 && p[1] == 0 && p[2] == 0)
        return IS_COPLANAR;
    else if (p[0] < 0 && p[1] < 0 && p[2] < 0)
        return IS_BEHIND;
    else
        return IS_SPANNING;
}

BSPTreeNode* BSPTreeBuilder::buildBSPTreeNode(ListPolygon polygons) {
    if (polygons.isEmpty())
        return nullptr;

    BSPTreeNode* tree = new BSPTreeNode;
    Polygon* root = polygons.getFromList();

    tree->partition = *root; // root->getHyperPlane();
    tree->polygons.addToList(root);
    ListPolygon front_list;
    ListPolygon back_list;

    Polygon* poly;
    while ((poly = polygons.getFromList()) != 0) {
        int result = classifyPolygon(&tree->partition, poly);
        switch (result) {
            case IS_COPLANAR:
                tree->polygons.addToList(poly);
                break;
            case IS_BEHIND:
                back_list.addToList(poly);
                break;
            case IS_INFRONT:
                front_list.addToList(poly);
                break;
            case IS_SPANNING:
                // TODO: implementar o split
                // Polygon *front_piece, *back_piece;
                // SplitPolygon(poly, &tree->partition, front_piece, back_piece);
                // back_list.addToList(back_piece);
                back_list.addToList(poly);
                break;
        }
    }
    tree->front = buildBSPTreeNode(front_list);
    tree->back = buildBSPTreeNode(back_list);
    return tree;
}
