#include "Triangle.hpp"

unsigned Triangle::serialMaster = 0;

Triangle::Triangle() : serial(++serialMaster) {
    for (int i = 0; i < 3; i++) {
        vertex[i].position = glm::vec3(0, 0, 0);
        vertex[i].normal = glm::vec3(0, 0, 0);
        vertex[i].texture = glm::vec2(0, 0);
        vertex[i].color = glm::vec4(0, 0, 0, 0);
    }
}

Triangle::Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) : serial(++serialMaster) {
    this->vertex[0].position = a;
    this->vertex[1].position = b;
    this->vertex[2].position = c;
}

Triangle::Triangle(const Triangle& _cpy) {
    serial = _cpy.serial;
    for (int i = 0; i < 3; i++) {
        this->vertex[i].position = _cpy.vertex[i].position;
        this->vertex[i].normal = _cpy.vertex[i].normal;
        this->vertex[i].texture = _cpy.vertex[i].texture;
        this->vertex[i].color = _cpy.vertex[i].color;
    }
}

glm::vec3 Triangle::normal() {
    glm::vec3 acc = glm::vec3(0, 0, 0);
    for (int i = 0; i < 3; i++)
        acc = acc + this->vertex[i].normal;
    return glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
}

glm::vec3 Triangle::verticeNormal() {
    glm::vec3 u = this->vertex[0].position - this->vertex[2].position;
    glm::vec3 v = this->vertex[0].position - this->vertex[1].position;
    glm::vec3 normal = glm::normalize(glm::cross(u, v));
    for (int i = 0; i < 3; i++) {
        this->vertex[i].normal = normal;
    }
    return normal;
}
