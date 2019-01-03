#include "BSPTree.h"

void BSPTree::draw(glm::vec3* eye, ListPolygon* finalFaces) { BSPTree::drawBSPTree(root, eye, finalFaces); }

float BSPTree::classify(glm::vec3* normal, glm::vec3* eye) {
    // TODO: Converir se e isto mesmo
    float dotVal1 = -(glm::dot(*normal, *normal));
    float p = (glm::dot(*normal, *eye)) + dotVal1;
    return p;
}

void BSPTree::drawBSPTree(BSPTreeNode* tree, glm::vec3* eye, ListPolygon* finalFaces) {
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
