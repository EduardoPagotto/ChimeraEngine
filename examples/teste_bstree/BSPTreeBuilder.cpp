#include "BSPTreeBuilder.hpp"

#define EPSILON 1e-3

template <class T> void swapFace(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

BSPTreeBuilder::BSPTreeBuilder(std::vector<Triangle>* _pListPolygon) { root = buildBSPTreeNode(_pListPolygon); }

glm::vec3 intersect(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& a, const glm::vec3& c) {
    float num = glm::dot(n, a);
    glm::vec3 cma = c - a;
    float denom = glm::dot(n, cma);
    float D = -glm::dot(n, p0);
    float t = -(num + D) / denom;

    glm::vec3 valor = a + t * (c - a);
    return aprox(valor);
}

glm::vec3 aprox(const glm::vec3& dado) {
    return glm::vec3((fabs(dado.x) < EPSILON) ? 0.0f : dado.x, (fabs(dado.y) < EPSILON) ? 0.0f : dado.y,
                     (fabs(dado.z) < EPSILON) ? 0.0f : dado.z);
}

void BSPTreeBuilder::splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Triangle* _partition,
                                   std::vector<Triangle>* _pListPolygon) {
    glm::vec3& a = _pTriangle->vertex[0].position; // getVertices()[0];
    glm::vec3& b = _pTriangle->vertex[1].position; // getVertices()[1];
    glm::vec3& c = _pTriangle->vertex[2].position; // getVertices()[2];
    float fa = fx.x;
    float fb = fx.y;
    float fc = fx.z;

    if (fa * fc >= 0) {
        swapFace(fb, fc);
        swapFace(b, c);
        swapFace(fa, fb);
        swapFace(a, b);
    } else if (fb * fc >= 0) {
        swapFace(fa, fc);
        swapFace(a, c);
        swapFace(fa, fb);
        swapFace(a, b);
    }

    glm::vec3 A = intersect(_partition->normal(), _partition->vertex[0].position, a, c);
    glm::vec3 B = intersect(_partition->normal(), _partition->vertex[0].position, b, c);

    Triangle T1(a, b, A);
    Triangle T2(b, B, A);
    Triangle T3(A, B, c);

    for (int i = 0; i < 3; i++) {
        T1.vertex[i].color = _pTriangle->vertex[i].color;
        T1.vertex[i].normal = _pTriangle->vertex[i].normal;
        T2.vertex[i].color = glm::vec4(0, 0, 1, 0);
        T2.vertex[i].normal = _pTriangle->vertex[i].normal;
        T3.vertex[i].color = glm::vec4(0, 1, 0, 0);
        T3.vertex[i].normal = _pTriangle->vertex[i].normal;
    }

    _pListPolygon->push_back(T1);
    _pListPolygon->push_back(T2);
    _pListPolygon->push_back(T3);
}

SIDE BSPTreeBuilder::ClassifyPolyTest(Triangle* Plane, Triangle* Poly) {
    // ref: http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
    int Infront = 0;
    int Behind = 0;
    int OnPlane = 0;
    float result;

    glm::vec3 vec1 = Plane->vertex[0].position; // getVertices();
    for (int a = 0; a < 3; a++) {

        glm::vec3 vec2 = Poly->vertex[a].position;
        glm::vec3 Direction = vec1 - vec2;
        result = glm::dot(Direction, Plane->normal());
        if (result > 0.001) {
            Behind++;
        } else if (result < -0.001) {
            Infront++;
        } else {
            OnPlane++;
            Infront++;
            Behind++;
        }
    }

    if (OnPlane == 3)
        return SIDE::IS_COPLANAR; // CP_ONPLANE;

    if (Behind == 3)
        return SIDE::IS_BEHIND; // CP_BACK;

    if (Infront == 3)
        return SIDE::IS_INFRONT; // CP_FRONT;

    return SIDE::IS_SPANNING; // CP_SPANNING;
}

glm::vec3 BSPTreeBuilder::classifyPolygon(Triangle* _pPartition, Triangle* _pTriangle) {
    float fs[3];
    for (int i = 0; i < 3; i++) {
        fs[i] = glm::dot(_pPartition->normal(), _pTriangle->vertex[i].position - _pPartition->vertex[0].position);
        if (fabs(fs[i]) < EPSILON)
            fs[i] = 0.0;
    }

    return glm::vec3(fs[0], fs[1], fs[2]);
}

BSPTreeNode* BSPTreeBuilder::buildBSPTreeNode(std::vector<Triangle>* _pListPolygon) {
    if (_pListPolygon->empty() == true)
        return nullptr;
    // tree->partition
    BSPTreeNode* tree = new BSPTreeNode(_pListPolygon->back());
    _pListPolygon->pop_back();
    tree->polygons.push_back(tree->partition);

    std::vector<Triangle> front_list;
    std::vector<Triangle> back_list;

    while (_pListPolygon->empty() == false) {

        Triangle poly = _pListPolygon->back();
        _pListPolygon->pop_back();

        SIDE teste = ClassifyPolyTest(&tree->partition, &poly);
        glm::vec3 result = classifyPolygon(&tree->partition, &poly);

        if (teste == SIDE::IS_BEHIND) // IS_BEHIND
            back_list.push_back(poly);
        else if (teste == SIDE::IS_INFRONT) // IS_INFRONT
            front_list.push_back(poly);
        else if (teste == SIDE::IS_COPLANAR) // IS_COPLANAR
            tree->polygons.push_back(poly);
        else // IS_SPANNING
            splitTriangle(result, &poly, &tree->partition, _pListPolygon);
    }
    tree->front = buildBSPTreeNode(&front_list);
    tree->back = buildBSPTreeNode(&back_list);
    return tree;
}
