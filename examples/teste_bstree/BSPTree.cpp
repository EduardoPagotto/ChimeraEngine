#include "BSPTree.h"
#include "Polygon.hpp"

float classify(glm::vec3* normal, glm::vec3* eye) {
    // TODO: Converir se e isto mesmo
    float dotVal1 = -(glm::dot(*normal, *normal));
    float p = (glm::dot(*normal, *eye)) + dotVal1;
    return p;
}

SIDE classifyPolygon(Polygon* _pPartition, Polygon* _pPolygon) {

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

void splitPolygon(Polygon* _poly, Polygon* _partition, Polygon* _front_piece, Polygon* _back_piece) {}

BSPTree* buildBSPTree(ListPolygon* polygons) {
    BSPTree* result = new BSPTree;
    result->root = buildBSPTreeNode(*polygons);
    return result;
};

BSPTreeNode* buildBSPTreeNode(ListPolygon polygons) {
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

void drawBSPTree(BSPTreeNode* tree, glm::vec3* eye, ListPolygon* finalFaces) {
    if (tree == nullptr)
        return;

    Polygon* p = nullptr;
    glm::vec3 normal = tree->partition.getFaceNormal();
    float result = classify(&normal, eye);
    if (result > 0) {
        drawBSPTree(tree->back, eye, finalFaces);

        tree->polygons.begin();
        while ((p = tree->polygons.next()) != NULL)
            finalFaces->addToList(p);
        tree->polygons.begin();
        // tree->polygons.DrawPolygons();

        drawBSPTree(tree->front, eye, finalFaces);

    } else if (result < 0) {
        drawBSPTree(tree->front, eye, finalFaces);

        tree->polygons.begin();
        while ((p = tree->polygons.next()) != NULL)
            finalFaces->addToList(p);
        tree->polygons.begin();
        // tree->polygons.DrawPolygonList();

        drawBSPTree(tree->back, eye, finalFaces);
    } else {
        // the eye point is on the partition plane...
        drawBSPTree(tree->front, eye, finalFaces);
        drawBSPTree(tree->back, eye, finalFaces);
    }
}