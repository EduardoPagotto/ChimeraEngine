#pragma once
#include "Frustum.hpp"
#include "array"

namespace Chimera {

enum class AabbBondery { BSW = 0, BSE = 1, TSW = 2, TSE = 3, BNW = 4, BNE = 5, TNW = 6, TNE = 7 };

class AABB { // ref: http://www.3dcpptutorials.sk/index.php?id=59
  protected:
    std::array<glm::vec3, 8> vertex{};

  private:
    glm::vec3 position{0.0f};
    glm::vec3 size{0.0f};

  public:
    AABB() noexcept = default;

    AABB(const AABB& _cpy) noexcept = default;

    AABB(const glm::vec3& min, const glm::vec3& max) noexcept { this->setBoundary(min, max); }

    virtual ~AABB() = default;

    inline const bool visible(const Frustum& _frustum) const noexcept { return _frustum.AABBVisible(vertex.data()); }

    inline const glm::vec3 getPosition() const { return position; }

    inline const glm::vec3 getSize() const { return size; }

    inline void setPosition(const glm::vec3& pos, const glm::vec3& size) noexcept {
        const glm::vec3 halfV = (size / 2.0f);
        setBoundary((pos - halfV), (pos + halfV)); // min and max
    }

    inline void setBoundary(const glm::vec3& min, const glm::vec3& max) noexcept {
        vertex[static_cast<int>(AabbBondery::BSW)] = glm::vec3(min.x, min.y, min.z); // 0 Minimal point (front)
        vertex[static_cast<int>(AabbBondery::BSE)] = glm::vec3(max.x, min.y, min.z); // 1
        vertex[static_cast<int>(AabbBondery::TSW)] = glm::vec3(min.x, max.y, min.z); // 2
        vertex[static_cast<int>(AabbBondery::TSE)] = glm::vec3(max.x, max.y, min.z); // 3
        vertex[static_cast<int>(AabbBondery::BNW)] = glm::vec3(min.x, min.y, max.z); // 4
        vertex[static_cast<int>(AabbBondery::BNE)] = glm::vec3(max.x, min.y, max.z); // 5
        vertex[static_cast<int>(AabbBondery::TNW)] = glm::vec3(min.x, max.y, max.z); // 6
        vertex[static_cast<int>(AabbBondery::TNE)] = glm::vec3(max.x, max.y, max.z); // 7 Maximal point (back)

        // TODO: Era half size ??
        this->size = getSizeMinMax(min, max);
        this->position = min + (this->size / 2.0f);
    }

    inline const bool intersects(const AABB& val) const noexcept {
        return (val.vertex[0].x > vertex[7].x || val.vertex[7].x < vertex[0].x || val.vertex[0].y < vertex[7].y ||
                val.vertex[7].y < vertex[0].y || val.vertex[0].z < vertex[7].z || val.vertex[7].z < vertex[0].z);
    }

    inline const bool contains(const glm::vec3& val) const noexcept {
        return !((val.x < vertex[0].x) || (val.y < vertex[0].y) || (val.z < vertex[0].z) || (val.x > vertex[7].x) ||
                 (val.y > vertex[7].y) || (val.z > vertex[7].z));
    }

    AABB transformation(const glm::mat4& transformation) const noexcept {
        // TODO: TESTAR!! ref: https://www.gamedev.net/forums/topic/673361-axis-aligned-boxes-and-rotations/
        glm::vec3 val, min, max;

        val = glm::vec3(transformation * glm::vec4(vertex[0], 1.0f));
        min = val;
        max = val;

        for (const glm::vec3& point : vertex) {
            val = glm::vec3(transformation * glm::vec4(point, 1.0f));
            min = glm::min(min, val);
            max = glm::max(max, val);
        }

        return AABB(min, max);
    }

    const glm::vec3* getAllVertex() const { return vertex.data(); }

    // TODO: verificar
    // inline const float distance(const Frustum& _frustum) const noexcept { return _frustum.AABBDistance(vertex.data()); }
};
} // namespace Chimera
