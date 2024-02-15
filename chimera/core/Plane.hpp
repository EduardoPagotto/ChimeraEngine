#pragma once
#include "OpenGLDefs.hpp"

namespace Chimera {

enum class SIDE { CP_ONPLANE, CP_FRONT, CP_BACK, CP_SPANNING };

class Plane {
  private:
    glm::vec3 m_point{0};  // vertice A
    glm::vec3 m_normal{0}; // plane calc cross product B and C across A
    float m_ND{0.0f};      // TODO: refazer e entender melhor
    int m_O{0};

  public:
    Plane() = default;

    Plane(const Plane& o) : m_point(o.m_point), m_normal(o.m_normal), m_ND(o.m_ND), m_O(o.m_O) {}

    Plane(const glm::vec3& position, const glm::vec3& normal) { this->set(position, normal); }

    virtual ~Plane() = default;

    void set(const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc) {

        m_point = pa;
        m_normal = glm::normalize(glm::cross(pb - pa, pc - pa));
        m_ND = dot(m_normal, pa);

        m_O = m_normal.z < 0.0f ? (m_normal.y < 0.0f ? (m_normal.x < 0.0f ? 0 : 1) : (m_normal.x < 0.0f ? 2 : 3))
                                : (m_normal.y < 0.0f ? (m_normal.x < 0.0f ? 4 : 5) : (m_normal.x < 0.0f ? 6 : 7));

        // SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "O Size: %d", O);
    }

    void set(const glm::vec3& point, const glm::vec3& normal) {

        m_point = point;   //_point->vertex[0].position;
        m_normal = normal; //_point->normal();
        m_ND = dot(m_normal, point);
        m_O = m_normal.z < 0.0f ? (m_normal.y < 0.0f ? (m_normal.x < 0.0f ? 0 : 1) : (m_normal.x < 0.0f ? 2 : 3))
                                : (m_normal.y < 0.0f ? (m_normal.x < 0.0f ? 4 : 5) : (m_normal.x < 0.0f ? 6 : 7));
    }

    inline const glm::vec3 getPoint() const { return this->m_point; }

    inline const glm::vec3 getNormal() const { return this->m_normal; }

    inline const bool collinearNormal(const glm::vec3& normal) const {

        const glm::vec3 sub = m_normal - normal;
        return (fabs(sub.x + sub.y + sub.z) < EPSILON);
    }

    const SIDE classifyPoint(const glm::vec3& point) const {

        const glm::vec3 dir = m_point - point;
        const float clipTest = glm::dot(dir, m_normal);

        if (fabs(clipTest) < EPSILON)
            return SIDE::CP_ONPLANE;

        if (clipTest < 0.0f)
            return SIDE::CP_FRONT;

        return SIDE::CP_BACK;
    }

    const SIDE classifyPoly(const glm::vec3& pA, const glm::vec3& pB, const glm::vec3& pC, glm::vec3& clipTest) const {

        uint8_t infront{0}, behind{0}, onPlane{0};

        clipTest.x = glm::dot((m_point - pA), m_normal); // Clip Test poin A
        clipTest.y = glm::dot((m_point - pB), m_normal); // Clip Test poin B
        clipTest.z = glm::dot((m_point - pC), m_normal); // Clip Test poin C

        for (uint8_t i = 0; i < 3; i++) {
            if (fabs(clipTest[i]) < EPSILON) {
                clipTest[i] = 0.0f;
                onPlane++;
                infront++;
                behind++;
            } else if (clipTest[i] > 0.0f) {
                behind++;
            } else { // clipTest[i] < 0.0f
                infront++;
            }
        }

        if (onPlane == 3)
            return SIDE::CP_ONPLANE;

        if (behind == 3)
            return SIDE::CP_BACK;

        if (infront == 3)
            return SIDE::CP_FRONT;

        return SIDE::CP_SPANNING;
    }

    const bool intersect(const glm::vec3& p0, const glm::vec3& p1, glm::vec3& intersection, float& percentage) const {

        const glm::vec3 direction = p1 - p0;
        const float linelength = glm::dot(direction, m_normal);
        if (fabsf(linelength) < 0.0001) // FIXME: EPISLON????
            return false;

        const glm::vec3 L1 = m_point - p0;
        const float dist_from_plane = glm::dot(L1, m_normal);
        percentage = dist_from_plane / linelength;

        if (percentage < 0.0f)
            return false;
        else if (percentage > 1.0f)
            return false;

        intersection = p0 + (direction * percentage);
        return true;
    }

    inline const bool AABBBehind(const glm::vec3* AABBVertices) const { return glm::dot(m_normal, AABBVertices[m_O]) < m_ND; }

    inline const float AABBDistance(const glm::vec3* AABBVertices) const { return glm::dot(m_normal, AABBVertices[m_O]); }
};
} // namespace Chimera
