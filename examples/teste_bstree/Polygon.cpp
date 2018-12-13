#include "Polygon.hpp"

Polygon::Polygon() {

    this->id = 0;
    for (int i = 0; i < 3; i++)
        vertices[i] = glm::vec3(0, 0, 0);

    normal = glm::vec3(0, 0, 0);
    color = glm::vec3(0, 0, 0);
}

Polygon::Polygon(const int& _id) {

    this->id = _id;
    for (int i = 0; i < 3; i++)
        vertices[i] = glm::vec3(0, 0, 0);

    normal = glm::vec3(0, 0, 0);
    color = glm::vec3(0, 0, 0);
}

Polygon::Polygon(const Polygon& _cpy) {

    id = _cpy.id;
    for (int i = 0; i < 3; i++)
        vertices[i] = _cpy.vertices[i];

    normal = _cpy.normal;
    color = _cpy.color;
}

void Polygon::setVertices(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    this->vertices[0] = a;
    this->vertices[1] = b;
    this->vertices[2] = c;
}

void Polygon::computeNormalVertices() {
    glm::vec3 e1 = this->vertices[0] - this->vertices[1];
    glm::vec3 e2 = this->vertices[2] - this->vertices[1];
    this->normal = glm::cross(e1, e2);
}
