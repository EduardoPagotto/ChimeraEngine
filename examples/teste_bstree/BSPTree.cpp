#include "BSPTree.h"

void BSPTree::draw(glm::vec3* eye, std::vector<Triangle>* _pArrayTriangle) {
    BSPTree::drawBSPTree(root, eye, _pArrayTriangle);
}

float BSPTree::classify(glm::vec3* normal, glm::vec3* eye) {
    // TODO: Converir se e isto mesmo
    float dotVal1 = -(glm::dot(*normal, *normal));
    float p = (glm::dot(*normal, *eye)) + dotVal1;
    return p;
}

void BSPTree::drawBSPTree(BSPTreeNode* tree, glm::vec3* eye, std::vector<Triangle>* _pArrayTriangle) {
    if (tree == nullptr)
        return;

    glm::vec3 normal = tree->partition.normal();
    float result = classify(&normal, eye);
    if (result > 0) {

        drawBSPTree(tree->back, eye, _pArrayTriangle);

        // tree->arrayTriangle.DrawPolygons(); // Abaixo equivale a esta linha
        for (auto it = tree->arrayTriangle.begin(); it != tree->arrayTriangle.end(); it++) {
            Triangle t = (*it);
            _pArrayTriangle->push_back(t);
        }

        drawBSPTree(tree->front, eye, _pArrayTriangle);

    } else if (result < 0) {
        drawBSPTree(tree->front, eye, _pArrayTriangle);

        // tree->arrayTriangle.DrawPolygons(); // Abaixo equivale a esta linha
        for (auto it = tree->arrayTriangle.begin(); it != tree->arrayTriangle.end(); it++) {
            Triangle t = (*it);
            _pArrayTriangle->push_back(t);
        }

        drawBSPTree(tree->back, eye, _pArrayTriangle);
    } else {
        // the eye point is on the partition plane...
        drawBSPTree(tree->front, eye, _pArrayTriangle);
        drawBSPTree(tree->back, eye, _pArrayTriangle);
    }
}
