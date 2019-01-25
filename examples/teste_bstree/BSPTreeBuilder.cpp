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

glm::vec2 retTex1(const glm::vec2& _p0, const glm::vec2& _p1) {
    return glm::vec2((_p0.x < _p1.x) ? _p1.x : _p0.x, (_p0.y < _p1.y) ? _p1.y : _p0.y);
}

void BSPTreeBuilder::splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Triangle* _partition,
                                   std::vector<Triangle>* _pListPolygon) {
    glm::vec3& a = _pTriangle->vertex[0].position;
    glm::vec3& b = _pTriangle->vertex[1].position;
    glm::vec3& c = _pTriangle->vertex[2].position;

    if (fx.x * fx.z >= 0) {
        swapFace(b, c);
        swapFace(a, b);
    } else if (fx.y * fx.z >= 0) {
        swapFace(a, c);
        swapFace(a, b);
    }

    // Testar ideia
    // Na textura se Xb - Xa > 0 entao Xb e [1,n] do contrario [0,n] para segmento de reta ab
    // Na textura se Yb - Ya > 0 entao Yb e [n,1] do contrario [n,0] para segmento de reta ab

    glm::vec3 A = intersect(_partition->normal(), _partition->vertex[0].position, a, c);
    glm::vec3 B = intersect(_partition->normal(), _partition->vertex[0].position, b, c);

    float propAC = (glm::distance(A, a) / glm::distance(a, c)); // razao da distancia entre a e A
    float propBC = (glm::distance(B, b) / glm::distance(b, c)); // razao da distancia entre b e B
    float propAB = (glm::distance(B, a) / glm::distance(a, b)); // razao da distancia entre a e B ??????

    float InterTexA = propAC * _pTriangle->vertex[1].texture.x;
    float InterTexB = propBC * _pTriangle->vertex[1].texture.y;
    // float InterTexU = propAB * _pTriangle->vertex[1].texture.y; // ??????

    Triangle T1(a, b, A);
    // T1.vertex[0].texture = _pTriangle->vertex[1].texture;
    T1.vertex[0].texture = retTex1(_pTriangle->vertex[0].texture, _pTriangle->vertex[1].texture);
    // 1.vertex[1].texture = _pTriangle->vertex[2].texture;
    T1.vertex[1].texture = retTex1(_pTriangle->vertex[1].texture, _pTriangle->vertex[2].texture);
    T1.vertex[2].texture = glm::vec2(InterTexA, _pTriangle->vertex[1].texture.y); // A

    //--

    Triangle T2(b, B, A);
    T2.vertex[0].texture = _pTriangle->vertex[2].texture; // b old c
    // T2.vertex[0].texture = retTex1(_pTriangle->vertex[0].texture, _pTriangle->vertex[2].texture);

    // Hipotenusa e cateto oposto para pegar o seno rad1
    float hypo = glm::distance(c, b);  // segmento de reta de c' ate b'
    float catOp = glm::distance(a, b); // segmento a' ate b'
    float rad1 = catOp / hypo;

    // Hipotenusa e seno para calcular o valor do cateto opposo (proporcao x da textura)
    float hypo2 = glm::distance(c, B);
    float seno1 = rad1;
    float val_final = seno1 * hypo2;         // valor da imagem do ponto B em Y (cateto oposto)
    float valxTexTemp = glm::distance(a, b); // tamanho total do Y
    float valxTex = val_final / valxTexTemp; // razao da textura em X ufa!!!

    T2.vertex[1].texture = glm::vec2(InterTexA, valxTex);                         // B
    T2.vertex[2].texture = glm::vec2(InterTexA, _pTriangle->vertex[1].texture.y); // A

    // --

    Triangle T3(A, B, c);
    T3.vertex[0].texture = glm::vec2(InterTexA, _pTriangle->vertex[1].texture.y); // A
    T3.vertex[1].texture = glm::vec2(InterTexA, valxTex);                         // B
    T3.vertex[2].texture = _pTriangle->vertex[0].texture;                         // c old a

    for (int i = 0; i < 3; i++) {
        T1.vertex[i].color = _pTriangle->vertex[i].color;
        T1.vertex[i].normal = _pTriangle->vertex[i].normal;

        T2.vertex[i].color = glm::vec3(0, 0, 1);
        // T2.vertex[i].color = _pTriangle->vertex[i].color;
        T2.vertex[i].normal = _pTriangle->vertex[i].normal;

        T3.vertex[i].color = glm::vec3(0, 1, 0);
        // T3.vertex[i].color = _pTriangle->vertex[i].color; // glm::vec3(0, 1, 0);
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
