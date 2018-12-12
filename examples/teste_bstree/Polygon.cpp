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