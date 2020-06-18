#include "chimera/render/Plane.hpp"
#include "chimera/render/VertexData.hpp"

namespace Chimera {
Plane::Plane() {}

Plane::Plane(const Plane& _cpy) {
    this->point = _cpy.point;
    this->normal = _cpy.normal;
    this->ND = _cpy.ND;
    this->O = _cpy.O;
}

Plane::Plane(const glm::vec3& _position, const glm::vec3& _normal) { this->set(_position, _normal); }

Plane::~Plane() {}

void Plane::set(const glm::vec3& pA, const glm::vec3& B, const glm::vec3& C) {
    point = pA;
    normal = glm::normalize(glm::cross(B - pA, C - pA));
    ND = dot(normal, pA);
    O = normal.z < 0.0f ? (normal.y < 0.0f ? (normal.x < 0.0f ? 0 : 1) : (normal.x < 0.0f ? 2 : 3))
                        : (normal.y < 0.0f ? (normal.x < 0.0f ? 4 : 5) : (normal.x < 0.0f ? 6 : 7));
}

void Plane::set(const glm::vec3& pA, const glm::vec3& _normal) {
    point = pA;       //_point->vertex[0].position;
    normal = _normal; //_point->normal();
    ND = dot(normal, pA);
    O = normal.z < 0.0f ? (normal.y < 0.0f ? (normal.x < 0.0f ? 0 : 1) : (normal.x < 0.0f ? 2 : 3))
                        : (normal.y < 0.0f ? (normal.x < 0.0f ? 4 : 5) : (normal.x < 0.0f ? 6 : 7));
}

bool Plane::collinearNormal(const glm::vec3& _normal) {
    glm::vec3 sub = this->normal - _normal;
    float result = (float)fabs(sub.x + sub.y + sub.z);
    if (result < EPSILON)
        return false;

    return true;
}

SIDE Plane::classifyPoint(glm::vec3* point) {
    // ref: http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
    glm::vec3 dir = this->point - (*point);
    float clipTest = glm::dot(dir, this->normal);

    if (fabs(clipTest) < EPSILON)
        SIDE::CP_ONPLANE;

    if (clipTest < 0.0f)
        return SIDE::CP_FRONT;

    return SIDE::CP_BACK;
}

SIDE Plane::classifyPoly(const glm::vec3& pA, const glm::vec3& pB, const glm::vec3& pC, glm::vec3* clipTest) {
    // ref: http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
    unsigned short infront = 0;
    unsigned short behind = 0;
    unsigned short onPlane = 0;
    float result[3];

    result[0] = glm::dot((this->point - pA), this->normal); // Clip Test poin A
    result[1] = glm::dot((this->point - pB), this->normal); // Clip Test poin B
    result[2] = glm::dot((this->point - pC), this->normal); // Clip Test poin C

    for (unsigned short a = 0; a < 3; a++) {
        if (fabs(result[a]) < EPSILON) {
            result[a] = 0.0f;
            onPlane++;
            infront++;
            behind++;
        } else if (result[a] > 0.0f) {
            behind++;
        } else { // result[a] < 0.0f
            infront++;
        }
    }

    clipTest->x = result[0];
    clipTest->y = result[1];
    clipTest->z = result[2];

    if (onPlane == 3)
        return SIDE::CP_ONPLANE;

    if (behind == 3)
        return SIDE::CP_BACK;

    if (infront == 3)
        return SIDE::CP_FRONT;

    return SIDE::CP_SPANNING;
}

bool Plane::intersect(glm::vec3* linestart, glm::vec3* lineend, glm::vec3* intersection, float* percentage) {

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

bool Plane::AABBBehind(const glm::vec3* AABBVertices) { return glm::dot(normal, AABBVertices[O]) < ND; }

float Plane::AABBDistance(const glm::vec3* AABBVertices) { return glm::dot(normal, AABBVertices[O]); }

} // namespace Chimera