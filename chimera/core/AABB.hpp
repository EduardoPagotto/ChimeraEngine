#pragma once
#include "Frustum.hpp"

namespace Chimera {

enum class AabbBondery { BSW = 0, BSE = 1, TSW = 2, TSE = 3, BNW = 4, BNE = 5, TNW = 6, TNE = 7 };

class AABB { // ref: http://www.3dcpptutorials.sk/index.php?id=59
  protected:
    glm::vec3 m_vertex[8];

  private:
    glm::vec3 m_position;
    glm::vec3 m_size;

  public:
    AABB() = default;

    AABB(const AABB& _cpy) {
        for (uint8_t i = 0; i < 8; i++)
            m_vertex[i] = _cpy.m_vertex[i];

        m_position = _cpy.m_position;
        m_size = _cpy.m_size;
    }

    AABB(const glm::vec3& _min, const glm::vec3& _max) { this->setBoundary(_min, _max); }

    virtual ~AABB() = default;

    inline const bool visible(const Frustum& _frustum) const { return _frustum.AABBVisible(m_vertex); }

    inline const float distance(const Frustum& _frustum) const { return _frustum.AABBDistance(m_vertex); }

    inline glm::vec3 getMax() const { return m_vertex[7]; }

    inline glm::vec3 getMin() const { return m_vertex[0]; }

    inline glm::vec3 getPosition() const { return m_position; }

    inline glm::vec3 getSize() const { return m_size; }

    void setPosition(const glm::vec3& pos, const glm::vec3& size) {

        // TODO: Era half size ??
        const glm::vec3 max = pos + (size / 2.0f);
        const glm::vec3 min = pos - (size / 2.0f);
        setBoundary(min, max);
    }

    void setBoundary(const glm::vec3& min, const glm::vec3& max) {

        m_vertex[(int)AabbBondery::BSW] = glm::vec3(min.x, min.y, min.z);
        m_vertex[(int)AabbBondery::BSE] = glm::vec3(max.x, min.y, min.z);
        m_vertex[(int)AabbBondery::TSW] = glm::vec3(min.x, max.y, min.z);
        m_vertex[(int)AabbBondery::TSE] = glm::vec3(max.x, max.y, min.z);
        m_vertex[(int)AabbBondery::BNW] = glm::vec3(min.x, min.y, max.z);
        m_vertex[(int)AabbBondery::BNE] = glm::vec3(max.x, min.y, max.z);
        m_vertex[(int)AabbBondery::TNW] = glm::vec3(min.x, max.y, max.z);
        m_vertex[(int)AabbBondery::TNE] = glm::vec3(max.x, max.y, max.z);

        // TODO: Era half size ??
        m_size = getSizeMinMax(min, max);
        m_position = min + (m_size / 2.0f);
    }

    const bool intersects(const AABB& aabb) const {
        return (aabb.m_vertex[0].x > m_vertex[7].x || // min x
                aabb.m_vertex[7].x < m_vertex[0].x || // max x
                aabb.m_vertex[0].y < m_vertex[7].y || // min y
                aabb.m_vertex[7].y < m_vertex[0].y || // max y
                aabb.m_vertex[0].z < m_vertex[7].z || // min z
                aabb.m_vertex[7].z < m_vertex[0].z);  // max z
    }

    const bool contains(const glm::vec3& point) const {
        if (point.x < m_vertex[0].x)
            return false;

        if (point.y < m_vertex[0].y)
            return false;

        if (point.z < m_vertex[0].z)
            return false;

        if (point.x > m_vertex[7].x)
            return false;

        if (point.y > m_vertex[7].y)
            return false;

        if (point.z > m_vertex[7].z)
            return false;

        return true;
    }

    AABB transformation(const glm::mat4& transformation) const {
        // TODO: TESTAR!! ref: https://www.gamedev.net/forums/topic/673361-axis-aligned-boxes-and-rotations/

        glm::vec3 val{glm::vec3(transformation * glm::vec4(m_vertex[0], 1.0f))};
        glm::vec3 min{val};
        glm::vec3 max{val};

        for (short i = 1; i < 8; i++) {
            val = glm::vec3(transformation * glm::vec4(m_vertex[i], 1.0f));
            min = glm::min(min, val);
            max = glm::max(max, val);
        }

        return AABB(min, max);
    }

    const glm::vec3& getVertex(int index) const { return m_vertex[index]; }

    const glm::vec3* getAllVertex() const { return m_vertex; }
};
} // namespace Chimera
