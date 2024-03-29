#include "chimera/core/space/AABB.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

AABB::AABB(const glm::vec3& _min, const glm::vec3& _max) { this->setBoundary(_min, _max); }

AABB::AABB(const AABB& _cpy) {
    for (short i = 0; i < 8; i++)
        vertex[i] = _cpy.vertex[i];

    position = _cpy.position;
    size = _cpy.size;
}

void AABB::setPosition(const glm::vec3& _pos, const glm::vec3& _size) {
    // TODO: Era half size ??
    glm::vec3 max = _pos + (_size / 2.0f);
    glm::vec3 min = _pos - (_size / 2.0f);
    setBoundary(min, max);
}

void AABB::setBoundary(const glm::vec3& _min, const glm::vec3& _max) {
    vertex[(int)AabbBondery::BSW] = glm::vec3(_min.x, _min.y, _min.z);
    vertex[(int)AabbBondery::BSE] = glm::vec3(_max.x, _min.y, _min.z);
    vertex[(int)AabbBondery::TSW] = glm::vec3(_min.x, _max.y, _min.z);
    vertex[(int)AabbBondery::TSE] = glm::vec3(_max.x, _max.y, _min.z);
    vertex[(int)AabbBondery::BNW] = glm::vec3(_min.x, _min.y, _max.z);
    vertex[(int)AabbBondery::BNE] = glm::vec3(_max.x, _min.y, _max.z);
    vertex[(int)AabbBondery::TNW] = glm::vec3(_min.x, _max.y, _max.z);
    vertex[(int)AabbBondery::TNE] = glm::vec3(_max.x, _max.y, _max.z);

    // TODO: Era half size ??
    size = getSizeMinMax(_min, _max);
    position = _min + (size / 2.0f);
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

// TODO: TESTAR!! ref: https://www.gamedev.net/forums/topic/673361-axis-aligned-boxes-and-rotations/
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