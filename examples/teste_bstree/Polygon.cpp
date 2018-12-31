#include "Polygon.hpp"

Polygon::Polygon() {

    this->id = 0;
    for (int i = 0; i < 3; i++) {
        vertices[i] = glm::vec3(0, 0, 0);
        normais[i] = glm::vec3(0, 0, 0);
    }

    faceNormal = glm::vec3(0, 0, 0);
    color = glm::vec3(0, 0, 0);
}

Polygon::Polygon(const int& _id) {

    this->id = _id;
    for (int i = 0; i < 3; i++) {
        vertices[i] = glm::vec3(0, 0, 0);
        normais[i] = glm::vec3(0, 0, 0);
    }

    faceNormal = glm::vec3(0, 0, 0);
    color = glm::vec3(0, 0, 0);
}

Polygon::Polygon(const Polygon& _cpy) {

    id = _cpy.id;
    for (int i = 0; i < 3; i++) {
        vertices[i] = _cpy.vertices[i];
        normais[i] = _cpy.normais[i];
    }

    faceNormal = _cpy.faceNormal;
    color = _cpy.color;
}

void Polygon::setVertices(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    this->vertices[0] = a;
    this->vertices[1] = b;
    this->vertices[2] = c;
}

void Polygon::computeFaceNormalsFromVertices() {

    glm::vec3 u = this->vertices[0] - this->vertices[2];
    glm::vec3 v = this->vertices[0] - this->vertices[1];
    glm::vec3 normal = glm::normalize(glm::cross(u, v));
    // glm::vec3 normal = glm::cross(u, v);

    this->normais[0] = this->normais[1] = this->normais[2] = normal;

    this->computeFaceNormalFromNormais();
}

void Polygon::setFaceNormal(const glm::vec3& n) {
    this->normais[0] = n;
    this->normais[1] = n;
    this->normais[2] = n;
    this->faceNormal = n;
}

void Polygon::setNormais(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {

    this->normais[0] = a;
    this->normais[1] = b;
    this->normais[2] = c;
    this->computeFaceNormalFromNormais();
}

void Polygon::computeFaceNormalFromNormais() {

    glm::vec3 acc = glm::vec3(0, 0, 0);
    for (int i = 0; i < 3; i++) {
        acc = acc + this->normais[i];
    }
    this->faceNormal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
}

// void Polygon::computeNormalVertices() {

//     glm::vec3 e1 = this->vertices[0] - this->vertices[1];
//     glm::vec3 e2 = this->vertices[2] - this->vertices[1];
//     this->normal = glm::cross(e1, e2);
//     this->normais[0] = this->normais[1] = this->normais[2] = this->normal;
// }
