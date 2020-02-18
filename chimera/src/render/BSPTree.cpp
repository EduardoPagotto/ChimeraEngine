#include "chimera/render/BSPTree.hpp"
#include <SDL2/SDL.h>

template <class T> void swapFace(T& a, T& b) {
    T c = b;
    b = a;
    a = c;
}

glm::vec3 aprox(const glm::vec3& dado) {
    return glm::vec3((fabs(dado.x) < EPSILON) ? 0.0f : dado.x, (fabs(dado.y) < EPSILON) ? 0.0f : dado.y,
                     (fabs(dado.z) < EPSILON) ? 0.0f : dado.z);
}

bool intersect(const glm::vec3& linestart, const glm::vec3& lineend, const glm::vec3& vertex, const glm::vec3& normal,
               glm::vec3& intersection, float& percentage) {

    glm::vec3 direction = lineend - linestart;
    float linelength = glm::dot(direction, normal);
    if (fabsf(linelength) < 0.0001)
        return false;

    glm::vec3 L1 = vertex - linestart;

    float dist_from_plane = glm::dot(L1, normal);
    percentage = dist_from_plane / linelength;

    if (percentage < 0.0f)
        return false;
    else if (percentage > 1.0f)
        return false;

    intersection = linestart + (direction * percentage);
    return true;
}

void splitTriangle(const glm::vec3& fx, Chimera::Triangle* _pTriangle, Chimera::Triangle* _partition,
                   std::vector<Chimera::Triangle>* _pListPolygon) {
    glm::vec3& a = _pTriangle->vertex[0].position;
    glm::vec3& b = _pTriangle->vertex[1].position;
    glm::vec3& c = _pTriangle->vertex[2].position;

    // acerto para vertex do tex final igualar a rotacao do triangulo
    Chimera::VertexData* pVertex_a = nullptr;
    Chimera::VertexData* pVertex_b = nullptr;
    Chimera::VertexData* pVertex_c = nullptr;

    // Normaliza Triangulo para que o corte do hiper-plano esteja nos segmentos de reta CA e CB (corte em a e b)
    if (fx.x * fx.z >= 0) { // corte em a e c
        swapFace(b, c);
        swapFace(a, b);

        pVertex_a = &_pTriangle->vertex[2]; // old c
        pVertex_b = &_pTriangle->vertex[0]; // old a
        pVertex_c = &_pTriangle->vertex[1]; // old b

    } else if (fx.y * fx.z >= 0) { // corte em b e c
        swapFace(a, c);
        swapFace(a, b);
        //--
        pVertex_a = &_pTriangle->vertex[1]; // old b
        pVertex_b = &_pTriangle->vertex[2]; // old c
        pVertex_c = &_pTriangle->vertex[0]; // old a

    } else { // Cortre em a e b

        pVertex_a = &_pTriangle->vertex[0]; // old a
        pVertex_b = &_pTriangle->vertex[1]; // old b
        pVertex_c = &_pTriangle->vertex[2]; // old c
    }

    float propAC = 0.0;
    float propBC = 0.0;
    glm::vec3 A, B;
    intersect(a, c, _partition->vertex[0].position, _partition->normal(), A, propAC);
    intersect(b, c, _partition->vertex[0].position, _partition->normal(), B, propBC);

    // PA texture coord
    glm::vec2 deltaA = (pVertex_c->texture - pVertex_a->texture) * propAC;
    glm::vec2 texA = pVertex_a->texture + deltaA;

    // PB texture coord
    glm::vec2 deltaB = (pVertex_c->texture - pVertex_b->texture) * propBC;
    glm::vec2 texB = pVertex_b->texture + deltaB;

    //-- T1
    Chimera::Triangle T1(a, b, A);
    T1.vertex[0].texture = pVertex_a->texture; // a old b
    T1.vertex[1].texture = pVertex_b->texture; // b old c
    T1.vertex[2].texture = texA;               // A

    //-- T2
    Chimera::Triangle T2(b, B, A);
    T2.vertex[0].texture = pVertex_b->texture; // b old c
    T2.vertex[1].texture = texB;               // B
    T2.vertex[2].texture = texA;               // A

    // -- T3
    Chimera::Triangle T3(A, B, c);
    T3.vertex[0].texture = texA;               // A
    T3.vertex[1].texture = texB;               // B
    T3.vertex[2].texture = pVertex_c->texture; // c old a

    for (int i = 0; i < 3; i++) {
        T1.vertex[i].normal = _pTriangle->vertex[i].normal;
        T2.vertex[i].normal = _pTriangle->vertex[i].normal;
        T3.vertex[i].normal = _pTriangle->vertex[i].normal;
    }

    _pListPolygon->push_back(T1);
    _pListPolygon->push_back(T2);
    _pListPolygon->push_back(T3);
}

SIDE classifyPoly(Chimera::Triangle* plane, Chimera::Triangle* poly, glm::vec3& _resultTest) {
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
        return SIDE::CP_ONPLANE;

    if (behind == 3)
        return SIDE::CP_BACK;

    if (infront == 3)
        return SIDE::CP_FRONT;

    return SIDE::CP_SPANNING;
}

BSPTreeNode* bsptreeBuild(std::vector<Chimera::Triangle>* _pListPolygon) {
    if (_pListPolygon->empty() == true)
        return nullptr;
    // tree->partition
    BSPTreeNode* tree = new BSPTreeNode(_pListPolygon->back());
    _pListPolygon->pop_back();
    tree->polygons.push_back(tree->partition);

    std::vector<Chimera::Triangle> front_list;
    std::vector<Chimera::Triangle> back_list;

    while (_pListPolygon->empty() == false) {

        Chimera::Triangle poly = _pListPolygon->back();
        _pListPolygon->pop_back();
        glm::vec3 result;
        SIDE teste = classifyPoly(&tree->partition, &poly, result);

        if (teste == SIDE::CP_BACK)
            back_list.push_back(poly);
        else if (teste == SIDE::CP_FRONT)
            front_list.push_back(poly);
        else if (teste == SIDE::CP_ONPLANE)
            tree->polygons.push_back(poly);
        else // CP_SPANNING
            splitTriangle(result, &poly, &tree->partition, _pListPolygon);
    }
    tree->front = bsptreeBuild(&front_list);
    tree->back = bsptreeBuild(&back_list);
    return tree;
}

//------PARSER METODOS------

SIDE classifyPoint(Chimera::Triangle* plane, glm::vec3* eye) {
    // ref: http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
    float result;
    glm::vec3* vec1 = (glm::vec3*)&plane->vertex[0];
    glm::vec3 dir = (*vec1) - (*eye);
    result = glm::dot(dir, plane->normal());

    if (result < -EPSILON)
        return SIDE::CP_FRONT;

    if (result > EPSILON)
        return SIDE::CP_BACK;

    return SIDE::CP_ONPLANE;
}

void drawPolygon(BSPTreeNode* tree, std::vector<Chimera::VertexData>* _pOutVertex, bool logdata, bool frontSide) {
    // tree->arrayTriangle.DrawPolygons(); // Abaixo equivale a esta linha
    for (auto it = tree->polygons.begin(); it != tree->polygons.end(); it++) {
        Chimera::Triangle t = (*it);

        // if (t.getSerial() == 10) // 8, 9, 10
        //     continue;

        _pOutVertex->push_back(t.vertex[0]);
        _pOutVertex->push_back(t.vertex[1]);
        _pOutVertex->push_back(t.vertex[2]);

        // FIXME: remover depois de concluir o algoritmo
        if (logdata == true) {
            if (frontSide == true)
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Face F: %d", t.getSerial());
            else
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Face B: %d", t.getSerial());
        }
    }
}

void traverseTree(BSPTreeNode* tree, glm::vec3* eye, std::vector<Chimera::VertexData>* _pOutVertex, bool logdata) {
    // ref: https://web.cs.wpi.edu/~matt/courses/cs563/talks/bsp/document.html
    if (tree == nullptr)
        return;

    SIDE result = classifyPoint(&tree->partition, eye);
    if (result == SIDE::CP_FRONT) {

        traverseTree(tree->back, eye, _pOutVertex, logdata);
        drawPolygon(tree, _pOutVertex, logdata, true);
        traverseTree(tree->front, eye, _pOutVertex, logdata);

    } else if (result == SIDE::CP_BACK) {

        traverseTree(tree->front, eye, _pOutVertex, logdata);
        drawPolygon(tree, _pOutVertex, logdata, false);
        traverseTree(tree->back, eye, _pOutVertex, logdata);

    } else { // result == SIDE::CP_ONPLANE
        // the eye point is on the partition plane...
        traverseTree(tree->front, eye, _pOutVertex, logdata);
        traverseTree(tree->back, eye, _pOutVertex, logdata);
    }
}

void bsptreeDraw(BSPTreeNode* _pRoot, glm::vec3* eye, std::vector<Chimera::VertexData>* _pOutVertex, bool logdata) {
    traverseTree(_pRoot, eye, _pOutVertex, logdata);
}
