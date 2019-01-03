#include "BSPTree.h"

void BSPTree::draw(glm::vec3* eye, ArrayTriangle* _pArrayTriangle) { BSPTree::drawBSPTree(root, eye, _pArrayTriangle); }

float BSPTree::classify(glm::vec3* normal, glm::vec3* eye) {
    // TODO: Converir se e isto mesmo
    float dotVal1 = -(glm::dot(*normal, *normal));
    float p = (glm::dot(*normal, *eye)) + dotVal1;
    return p;
}

void BSPTree::drawBSPTree(BSPTreeNode* tree, glm::vec3* eye, ArrayTriangle* _pArrayTriangle) {
    if (tree == nullptr)
        return;

    Triangle* t = nullptr;
    glm::vec3 normal = tree->partition.getFaceNormal();
    float result = classify(&normal, eye);
    if (result > 0) {
        drawBSPTree(tree->back, eye, _pArrayTriangle);

        tree->arrayTriangle.begin();
        while ((t = tree->arrayTriangle.next()) != NULL)
            _pArrayTriangle->addToList(t);

        tree->arrayTriangle.begin();
        // tree->arrayTriangle.DrawPolygons();

        drawBSPTree(tree->front, eye, _pArrayTriangle);

    } else if (result < 0) {
        drawBSPTree(tree->front, eye, _pArrayTriangle);

        tree->arrayTriangle.begin();
        while ((t = tree->arrayTriangle.next()) != NULL)
            _pArrayTriangle->addToList(t);

        tree->arrayTriangle.begin();
        // tree->arrayTriangle.DrawPolygonList();

        drawBSPTree(tree->back, eye, _pArrayTriangle);
    } else {
        // the eye point is on the partition plane...
        drawBSPTree(tree->front, eye, _pArrayTriangle);
        drawBSPTree(tree->back, eye, _pArrayTriangle);
    }
}
