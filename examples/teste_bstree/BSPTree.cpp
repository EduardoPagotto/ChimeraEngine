#include "BSPTree.h"

void BSPTree::draw(glm::vec3* eye, std::vector<Triangle>* _pListPolygon) {
    BSPTree::drawBSPTree(root, eye, _pListPolygon);
}

float BSPTree::classify(glm::vec3* normal, glm::vec3* eye) {
    // TODO: Converir se e isto mesmo
    float dotVal1 = -(glm::dot(*normal, *normal));
    float p = (glm::dot(*normal, *eye)) + dotVal1;
    return p;
}

void BSPTree::drawBSPTree(BSPTreeNode* tree, glm::vec3* eye, std::vector<Triangle>* _pListPolygon) {
    if (tree == nullptr)
        return;

    glm::vec3 normal = tree->partition.normal();
    float result = classify(&normal, eye);
    if (result > 0) {

        drawBSPTree(tree->back, eye, _pListPolygon);

        // tree->arrayTriangle.DrawPolygons(); // Abaixo equivale a esta linha
        for (auto it = tree->polygons.begin(); it != tree->polygons.end(); it++) {
            Triangle t = (*it);
            _pListPolygon->push_back(t);
        }

        drawBSPTree(tree->front, eye, _pListPolygon);

    } else if (result < 0) {
        drawBSPTree(tree->front, eye, _pListPolygon);

        // tree->arrayTriangle.DrawPolygons(); // Abaixo equivale a esta linha
        for (auto it = tree->polygons.begin(); it != tree->polygons.end(); it++) {
            Triangle t = (*it);
            _pListPolygon->push_back(t);
        }

        drawBSPTree(tree->back, eye, _pListPolygon);
    } else {
        // the eye point is on the partition plane...
        drawBSPTree(tree->front, eye, _pListPolygon);
        drawBSPTree(tree->back, eye, _pListPolygon);
    }
}
