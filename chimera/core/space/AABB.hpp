#pragma once
#include "Frustum.hpp"

namespace Chimera {

enum class AabbBondery { BSW = 0, BSE = 1, TSW = 2, TSE = 3, BNW = 4, BNE = 5, TNW = 6, TNE = 7 };

class AABB { // ref: http://www.3dcpptutorials.sk/index.php?id=59
  protected:
    glm::vec3 vertex[8]{glm::vec3(0.0f)};

  private:
    glm::vec3 position;
    glm::vec3 size;

  public:
    AABB() = default;

    AABB(const AABB& _cpy) = default;

    AABB(const glm::vec3& min, const glm::vec3& max) { this->setBoundary(min, max); }

    virtual ~AABB() = default;

    inline const bool visible(const Frustum& _frustum) const { return _frustum.AABBVisible(vertex); }

    inline const float distance(const Frustum& _frustum) const { return _frustum.AABBDistance(vertex); }

    inline const glm::vec3 getMax() const { return vertex[7]; }

    inline const glm::vec3 getMin() const { return vertex[0]; }

    inline const glm::vec3 getPosition() const { return position; }

    inline const glm::vec3 getSize() const { return size; }

    inline void setPosition(const glm::vec3& pos, const glm::vec3& size) {
        const glm::vec3 halfV = (size / 2.0f);
        setBoundary((pos - halfV), (pos + halfV)); // min and max
    }

    inline void setBoundary(const glm::vec3& min, const glm::vec3& max) {
        vertex[static_cast<int>(AabbBondery::BSW)] = glm::vec3(min.x, min.y, min.z); // 0
        vertex[static_cast<int>(AabbBondery::BSE)] = glm::vec3(max.x, min.y, min.z); // 1
        vertex[static_cast<int>(AabbBondery::TSW)] = glm::vec3(min.x, max.y, min.z); // 2
        vertex[static_cast<int>(AabbBondery::TSE)] = glm::vec3(max.x, max.y, min.z); // 3
        vertex[static_cast<int>(AabbBondery::BNW)] = glm::vec3(min.x, min.y, max.z); // 4
        vertex[static_cast<int>(AabbBondery::BNE)] = glm::vec3(max.x, min.y, max.z); // 5
        vertex[static_cast<int>(AabbBondery::TNW)] = glm::vec3(min.x, max.y, max.z); // 6
        vertex[static_cast<int>(AabbBondery::TNE)] = glm::vec3(max.x, max.y, max.z); // 7

        // TODO: Era half size ??
        this->size = getSizeMinMax(min, max);
        this->position = min + (this->size / 2.0f);
    }

    inline const bool intersects(const AABB& _aabb) const {
        return (_aabb.vertex[0].x > this->vertex[7].x || _aabb.vertex[7].x < this->vertex[0].x || _aabb.vertex[0].y < this->vertex[7].y ||
                _aabb.vertex[7].y < this->vertex[0].y || _aabb.vertex[0].z < this->vertex[7].z || _aabb.vertex[7].z < this->vertex[0].z);
    }

    inline const bool contains(const glm::vec3& _point) const {
        return !((_point.x < vertex[0].x) || (_point.y < vertex[0].y) || (_point.z < vertex[0].z) || (_point.x > vertex[7].x) ||
                 (_point.y > vertex[7].y) || (_point.z > vertex[7].z));
    }

    AABB transformation(const glm::mat4& transformation) const {
        // TODO: TESTAR!! ref: https://www.gamedev.net/forums/topic/673361-axis-aligned-boxes-and-rotations/
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

    const glm::vec3& getVertex(int index) const { return vertex[index]; }

    const glm::vec3* getAllVertex() const { return vertex; }
};
} // namespace Chimera
