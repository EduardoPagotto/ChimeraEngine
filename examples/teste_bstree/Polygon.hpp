
#ifndef __POLYGON__HPP
#define __POLYGON__HPP

#include <glm/glm.hpp>

struct Polygon {
    int id;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec3 vertices[3];
};

void setPolygonVerticesAndCalcNormal(struct Polygon* p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
void setPolygonVertices(struct Polygon* p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
void computePolygonNormal(struct Polygon* p);

#endif