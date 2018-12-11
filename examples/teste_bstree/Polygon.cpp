#include "Polygon.hpp"

Polygon::Polygon() {}

Polygon::~Polygon() {}

void Polygon::setNormals(glm::vec3 n) { Polygon::normal = n; }

void Polygon::setVertices(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {

    Polygon::vertices[0] = p1;
    Polygon::vertices[1] = p2;
    Polygon::vertices[2] = p3;
}

glm::vec3 Polygon::getColor() { return Polygon::color; }

void Polygon::setColor(glm::vec3 col) { Polygon::color = col; }

glm::vec3* Polygon::getVertices() { return Polygon::vertices; }

glm::vec3 Polygon::getNormals() { return Polygon::normal; }