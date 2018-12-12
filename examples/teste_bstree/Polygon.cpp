#include "Polygon.hpp"

void setPolygonVertices(struct Polygon* p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    p->vertices[0] = a;
    p->vertices[1] = b;
    p->vertices[2] = c;
};

// computes normal
void computePolygonNormal(struct Polygon* p) {
    glm::vec3 e1 = p->vertices[0] - p->vertices[1];
    glm::vec3 e2 = p->vertices[2] - p->vertices[1];
    p->normal = glm::cross(e1, e2);
}

void setPolygonVerticesAndCalcNormal(struct Polygon* p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    setPolygonVertices(p, a, b, c);
    computePolygonNormal(p);
}

struct Polygon* createCpyPolygon(struct Polygon* _pSrc) {
    Polygon* p = new Polygon;
    cpyPolygon(_pSrc, p);
    return p;
}

void cpyPolygon(struct Polygon* _pSrc, struct Polygon* _pDst) {

    for (int i = 0; i < 3; i++)
        _pDst->vertices[i] = _pSrc->vertices[i];

    _pDst->id = _pSrc->id;
    _pDst->normal = _pSrc->normal;
    _pDst->color = _pSrc->color;
}