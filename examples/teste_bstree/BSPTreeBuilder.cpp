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
    glm::vec3& a = _pTriangle->vertex[0].position;
    glm::vec3& b = _pTriangle->vertex[1].position;
    glm::vec3& c = _pTriangle->vertex[2].position;

    if (fx.x * fx.z >= 0) {
        // swapFace(fx.y, fx.z);
        swapFace(b, c);
        // swapFace(fx.x, fx.y);
        swapFace(a, b);
    } else if (fx.y * fx.z >= 0) {
        // swapFace(fx.x, fx.z);
        swapFace(a, c);
        // swapFace(fx.x, fx.y);
        swapFace(a, b);
    }

    glm::vec3 A = intersect(_partition->normal(), _partition->vertex[0].position, a, c);
    glm::vec3 B = intersect(_partition->normal(), _partition->vertex[0].position, b, c);

    Triangle T1(a, b, A);
    T1.vertex[0].texture = _pTriangle->vertex[1].texture;
    T1.vertex[1].texture = _pTriangle->vertex[2].texture;

    float tot = glm::distance(a, c);
    float pro = glm::distance(A, a);
    float porcentagem = (pro) / tot;
    float valFinalA = porcentagem * _pTriangle->vertex[1].texture.x;
    T1.vertex[2].texture = glm::vec2(valFinalA, _pTriangle->vertex[1].texture.y);

    Triangle T2(b, B, A);
    T2.vertex[0].texture = _pTriangle->vertex[2].texture;
    tot = glm::distance(b, c);
    pro = glm::distance(B, b);
    porcentagem = (pro) / tot;
    float valFinalB = porcentagem * _pTriangle->vertex[1].texture.y;
    T2.vertex[1].texture = glm::vec2(valFinalA, valFinalB);
    T2.vertex[2].texture = glm::vec2(valFinalA, _pTriangle->vertex[1].texture.y);

    Triangle T3(A, B, c);
    T3.vertex[0].texture = glm::vec2(valFinalA, _pTriangle->vertex[1].texture.y);
    T3.vertex[1].texture = glm::vec2(valFinalA, valFinalB);
    T3.vertex[2].texture = _pTriangle->vertex[0].texture;

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

SIDE BSPTreeBuilder::classifyPoly(Triangle* plane, Triangle* poly, glm::vec3& _resultTest) {
    // ref: http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
    unsigned short infront = 0;
    unsigned short behind = 0;
    unsigned short onPlane = 0;
    float result[3];

    for (unsigned short a = 0; a < 3; a++) {
        glm::vec3 direction = plane->vertex[0].position - poly->vertex[a].position;
        result[a] = glm::dot(direction, plane->normal());
        if (result[a] > EPSILON) {
            behind++;
        } else if (result[a] < -EPSILON) {
            infront++;
        } else {
            onPlane++;
            infront++;
            behind++;
        }
    }

    _resultTest.x = result[0];
    _resultTest.y = result[1];
    _resultTest.z = result[2];

    if (onPlane == 3)
        return SIDE::IS_COPLANAR; // CP_ONPLANE;

    if (behind == 3)
        return SIDE::IS_BEHIND; // CP_BACK;

    if (infront == 3)
        return SIDE::IS_INFRONT; // CP_FRONT;

    return SIDE::IS_SPANNING; // CP_SPANNING;
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
        glm::vec3 result;
        SIDE teste = classifyPoly(&tree->partition, &poly, result);

        if (teste == SIDE::IS_BEHIND)
            back_list.push_back(poly);
        else if (teste == SIDE::IS_INFRONT)
            front_list.push_back(poly);
        else if (teste == SIDE::IS_COPLANAR)
            tree->polygons.push_back(poly);
        else // IS_SPANNING
            splitTriangle(result, &poly, &tree->partition, _pListPolygon);
    }
    tree->front = buildBSPTreeNode(&front_list);
    tree->back = buildBSPTreeNode(&back_list);
    return tree;
}
