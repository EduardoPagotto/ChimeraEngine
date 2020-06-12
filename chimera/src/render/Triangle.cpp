#include "chimera/render/Triangle.hpp"

namespace Chimera {

unsigned Triangle::serialMaster = 0;

Triangle::Triangle(const VertexData& va, const VertexData& vb, const VertexData& vc) : serial(serialMaster++) {
    this->vertex[0] = va;
    this->vertex[1] = vb;
    this->vertex[2] = vc;
}

Triangle::Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) : serial(serialMaster++) {
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
    }
}

glm::vec3 Triangle::normal() const {
    glm::vec3 acc = glm::vec3(0, 0, 0);
    for (int i = 0; i < 3; i++)
        acc = acc + this->vertex[i].normal;
    return glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
}

void Triangle::generateNormal() {
    glm::vec3 u = this->vertex[1].position - this->vertex[0].position;
    glm::vec3 v = this->vertex[2].position - this->vertex[0].position;
    glm::vec3 normal = glm::normalize(glm::cross(u, v));

    for (int i = 0; i < 3; i++)
        this->vertex[i].normal = normal;
}

// PlanePoint::PlanePoint(glm::vec3* _position, glm::vec3* _normal) {
//     point = *_position; //_point->vertex[0].position;
//     normal = *_normal;  //_point->normal();
//     // d = point.x + point.y + point.z;
//     // s = glm::length(normal);
// }

PlanePoint::PlanePoint(Triangle* _point) {
    point = _point->vertex[0].position;
    normal = _point->normal();
    // d = point.x + point.y + point.z;
    // s = glm::length(normal);
}

// float PlanePoint::distance(glm::vec3* point) {

//     float a = (normal.x * point->x) + (normal.y * point->y) + (normal.z * point->z) - d;
//     float r = a / s;
//     return r;
// }

SIDE PlanePoint::classifyPoint(glm::vec3* point) {
    // ref: http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
    float result;
    glm::vec3 dir = this->point - (*point);
    result = glm::dot(dir, this->normal);

    if (result < -EPSILON)
        return SIDE::CP_FRONT;

    if (result > EPSILON)
        return SIDE::CP_BACK;

    return SIDE::CP_ONPLANE;
}

SIDE PlanePoint::classifyPoly(Triangle* poly, glm::vec3* distance) {
    // ref: http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
    unsigned short infront = 0;
    unsigned short behind = 0;
    unsigned short onPlane = 0;
    float result[3];

    for (unsigned short a = 0; a < 3; a++) {
        glm::vec3 direction = this->point - poly->vertex[a].position;
        result[a] = glm::dot(direction, this->normal);
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

    distance->x = result[0];
    distance->y = result[1];
    distance->z = result[2];

    if (onPlane == 3)
        return SIDE::CP_ONPLANE;

    if (behind == 3)
        return SIDE::CP_BACK;

    if (infront == 3)
        return SIDE::CP_FRONT;

    return SIDE::CP_SPANNING;
}

bool PlanePoint::intersect(glm::vec3* linestart, glm::vec3* lineend, glm::vec3* intersection, float* percentage) {

    glm::vec3 direction = (*lineend) - (*linestart);
    float linelength = glm::dot(direction, this->normal);
    if (fabsf(linelength) < 0.0001)
        return false;

    glm::vec3 L1 = this->point - (*linestart);

    float dist_from_plane = glm::dot(L1, this->normal);
    *percentage = dist_from_plane / linelength;

    if (*percentage < 0.0f)
        return false;
    else if (*percentage > 1.0f)
        return false;

    *intersection = (*linestart) + (direction * (*percentage));
    return true;
}
} // namespace Chimera
