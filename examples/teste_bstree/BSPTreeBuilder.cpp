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

void BSPTreeBuilder::splitTriangle(const glm::ivec3& fx, Triangle* _pTriangle, Triangle* _partition,
                                   ArrayTriangle* _pArrayTriangle) {
    glm::vec3& a = _pTriangle->getVertices()[0];
    glm::vec3& b = _pTriangle->getVertices()[1];
    glm::vec3& c = _pTriangle->getVertices()[2];
    float fa = fx.x;
    float fb = fx.y;
    float fc = fx.z;

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

    Triangle T1(a, b, A);
    Triangle T2(b, B, A);
    Triangle T3(A, B, c);

    _pArrayTriangle->addToList(&T1);
    _pArrayTriangle->addToList(&T2);
    _pArrayTriangle->addToList(&T3);
}

glm::ivec3 BSPTreeBuilder::classifyPolygon(Triangle* _pPartition, Triangle* _pTriangle) {
    float fs[3];
    for (int i = 0; i < 3; i++) {
        fs[i] = glm::dot(_pPartition->getFaceNormal(), _pTriangle->getVertices()[i] - _pPartition->getVertices()[0]);
        if (fabs(fs[i]) < EPSILON)
            fs[i] = 0.0;
    }

    return glm::ivec3(fs[0], fs[1], fs[2]);
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
        glm::ivec3 result = classifyPolygon(&tree->partition, t);
        if (result.x < 0 && result.y < 0 && result.z < 0) // IS_BEHIND
            back_list.addToList(t);
        else if (result.x > 0 && result.y > 0 && result.z > 0) // IS_INFRONT
            front_list.addToList(t);
        else if (result.x == 0 && result.y == 0 && result.z == 0) // IS_COPLANAR
            tree->arrayTriangle.addToList(t);
        else { // IS_SPANNING
            back_list.addToList(t);
            // splitTriangle(result, t, &tree->partition, &_arrayTriangle);
        }
    }
    tree->front = buildBSPTreeNode(front_list);
    tree->back = buildBSPTreeNode(back_list);
    return tree;
}
