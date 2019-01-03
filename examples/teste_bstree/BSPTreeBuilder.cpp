#include "BSPTreeBuilder.hpp"

#define EPSILON 1e-3

template <class T> void swap(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

BSPTreeBuilder::BSPTreeBuilder(ArrayTriangle* _arrayTriangle) { root = buildBSPTreeNode(*_arrayTriangle); }

glm::vec3 intersect(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& a, const glm::vec3& c) {
    float num = glm::dot(n, a);
    glm::vec3 cma = c - a;
    float denom = glm::dot(n, cma);
    float D = -glm::dot(n, p0);
    float t = -(num + D) / denom;
    return a + t * (c - a);
}

float BSPTreeBuilder::f(const glm::vec3& p, Triangle* _pPartition) {
    glm::vec3 n = _pPartition->getFaceNormal();
    return glm::dot(n, p - _pPartition->getVertices()[0]);
}

void BSPTreeBuilder::splitTriangle(Triangle* _pTriangle, Triangle* _partition, ArrayTriangle* _pArrayTriangle) {

    glm::vec3& a = _pTriangle->getVertices()[0];
    glm::vec3& b = _pTriangle->getVertices()[1];
    glm::vec3& c = _pTriangle->getVertices()[2];
    float fa = BSPTreeBuilder::f(a, _partition);
    float fb = BSPTreeBuilder::f(b, _partition);
    float fc = BSPTreeBuilder::f(c, _partition);
    if (fabs(fa) < EPSILON)
        fa = 0.0;
    if (fabs(fb) < EPSILON)
        fb = 0.0;
    if (fabs(fc) < EPSILON)
        fc = 0.0;

    if (fa * fc >= 0) {
        swap(fb, fc);
        swap(b, c);
        swap(fa, fb);
        swap(a, b);
    } else if (fb * fc >= 0) {
        swap(fa, fc);
        swap(a, c);
        swap(fa, fb);
        swap(a, b);
    }

    glm::vec3 A = intersect(_partition->getFaceNormal(), _partition->getVertices()[0], a, c);
    glm::vec3 B = intersect(_partition->getFaceNormal(), _partition->getVertices()[0], b, c);

    Triangle T1(a, b, A); // TreeTriangle T1(a, b, A);
    Triangle T2(b, B, A); // TreeTriangle T2(b, B, A);
    Triangle T3(A, B, c); // TreeTriangle T3(A, B, c);

    _pArrayTriangle->addToList(&T1); // to_add.push_back(T1);
    _pArrayTriangle->addToList(&T2); // to_add.push_back(T2);
    _pArrayTriangle->addToList(&T3); // to_add.push_back(T3);
}

SIDE BSPTreeBuilder::classifyPolygon(Triangle* _pPartition, Triangle* _pTriangle) {

    glm::vec3 n1 = _pPartition->getFaceNormal();
    glm::vec3 v1 = _pPartition->getVertices()[0];
    float dotVal1 = -(glm::dot(n1, v1));

    float p[3];
    for (int i = 0; i < 3; i++)
        p[i] = glm::dot(n1, _pTriangle->getVertices()[i]) + dotVal1;

    if (p[0] > 0 && p[1] > 0 && p[2] > 0)
        return IS_INFRONT;
    else if (p[0] == 0 && p[1] == 0 && p[2] == 0)
        return IS_COPLANAR;
    else if (p[0] < 0 && p[1] < 0 && p[2] < 0)
        return IS_BEHIND;
    else
        return IS_SPANNING;
}

BSPTreeNode* BSPTreeBuilder::buildBSPTreeNode(ArrayTriangle _arrayTriangle) {
    if (_arrayTriangle.isEmpty())
        return nullptr;

    BSPTreeNode* tree = new BSPTreeNode;
    Triangle* root = _arrayTriangle.getFromList();

    tree->partition = *root; // root->getHyperPlane();
    tree->arrayTriangle.addToList(root);
    ArrayTriangle front_list;
    ArrayTriangle back_list;

    Triangle* t;
    while ((t = _arrayTriangle.getFromList()) != 0) {
        int result = classifyPolygon(&tree->partition, t);
        switch (result) {
            case IS_COPLANAR:
                tree->arrayTriangle.addToList(t);
                break;
            case IS_BEHIND:
                back_list.addToList(t);
                break;
            case IS_INFRONT:
                front_list.addToList(t);
                break;
            case IS_SPANNING:
                // TODO: implementar o split
                // Triangle *front_piece, *back_piece;
                // splitTriangle(t, &tree->partition, front_piece, back_piece);
                // back_list.addToList(back_piece);
                back_list.addToList(t);
                // splitTriangle(t, &tree->partition, &_arrayTriangle);
                break;
        }
    }
    tree->front = buildBSPTreeNode(front_list);
    tree->back = buildBSPTreeNode(back_list);
    return tree;
}
