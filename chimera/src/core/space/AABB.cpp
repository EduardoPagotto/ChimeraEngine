#include "chimera/core/space/AABB.hpp"

namespace Chimera {

AABB::AABB() {} // ref: http://www.3dcpptutorials.sk/index.php?id=59

AABB::AABB(const glm::vec3& _min, const glm::vec3& _max) { this->setBoundary(_min, _max); }

AABB::AABB(const AABB& _cpy) {
    for (short i = 0; i < 8; i++)
        vertex[i] = _cpy.vertex[i];

    position = _cpy.position;
    size = _cpy.size;
}

AABB::~AABB() {}

void AABB::setPosition(const glm::vec3& _pos, const glm::vec3& _size) {
    glm::vec3 max = _pos + _size;
    glm::vec3 min = _pos - _size;
    setBoundary(max, min);
}

void AABB::setBoundary(const glm::vec3& _min, const glm::vec3& _max) {
    vertex[0] = glm::vec3(_min.x, _min.y, _min.z);
    vertex[1] = glm::vec3(_max.x, _min.y, _min.z);
    vertex[2] = glm::vec3(_min.x, _max.y, _min.z);
    vertex[3] = glm::vec3(_max.x, _max.y, _min.z);
    vertex[4] = glm::vec3(_min.x, _min.y, _max.z);
    vertex[5] = glm::vec3(_max.x, _min.y, _max.z);
    vertex[6] = glm::vec3(_min.x, _max.y, _max.z);
    vertex[7] = glm::vec3(_max.x, _max.y, _max.z);

    size = glm::vec3((glm::abs(_max.x) + glm::abs(_min.x)) / 2,  // X
                     (glm::abs(_max.y) + glm::abs(_min.y)) / 2,  // Y
                     (glm::abs(_max.z) + glm::abs(_min.z)) / 2); // Z

    position = _min + size;
}

bool AABB::intersects(const AABB& _aabb) const {
    return (_aabb.vertex[0].x > this->vertex[7].x || // min x
            _aabb.vertex[7].x < this->vertex[0].x || // max x
            _aabb.vertex[0].y < this->vertex[7].y || // min y
            _aabb.vertex[7].y < this->vertex[0].y || // max y
            _aabb.vertex[0].z < this->vertex[7].z || // min z
            _aabb.vertex[7].z < this->vertex[0].z);  // max z
}

bool AABB::contains(const glm::vec3& _point) const {
    if (_point.x < vertex[0].x)
        return false;
    if (_point.y < vertex[0].y)
        return false;
    if (_point.z < vertex[0].z)
        return false;

    if (_point.x > vertex[7].x)
        return false;
    if (_point.y > vertex[7].y)
        return false;
    if (_point.z > vertex[7].z)
        return false;

    return true;
}

bool AABB::visible(const Frustum& _frustum) const { return _frustum.AABBVisible(vertex); }

float AABB::distance(const Frustum& _frustum) const { return _frustum.AABBDistance(vertex); }

// TODO: TESTAR!!!!!
// ref: https://www.gamedev.net/forums/topic/673361-axis-aligned-boxes-and-rotations/
AABB AABB::transformation(const glm::mat4& transformation) const {

    glm::vec3 val, min, max;

    val = glm::vec3(transformation * glm::vec4(vertex[0], 1.0f));
    min = val;
    max = val;

    for (short i = 1; i < 8; i++) {
        val = glm::vec3(transformation * glm::vec4(vertex[i], 1.0f));
        min = glm::min(min, val);
        max = glm::max(max, val);
    }

    return AABB(min, max);
}
} // namespace Chimera