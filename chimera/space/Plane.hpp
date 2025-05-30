#pragma once
#include "space.hpp"
#include <array>

namespace ce {

    enum class SIDE { CP_ONPLANE, CP_FRONT, CP_BACK, CP_SPANNING };

    class Plane {
      private:
        glm::vec3 point{0.0f};  // vertice A
        glm::vec3 normal{0.0f}; // plane calc cross product B and C across A
        float ND{0.0f};
        int O{0};

      public:
        explicit Plane() noexcept = default;

        explicit Plane(const Plane& o) noexcept = default;

        explicit Plane(const glm::vec3& point, const glm::vec3& normal) noexcept : point(point), normal(normal) {
            this->calcND();
        }

        explicit Plane(const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc) noexcept : point(pa) {
            normal = glm::normalize(glm::cross(pb - pa, pc - pa));
            this->calcND();
        }

        virtual ~Plane() noexcept = default;

        Plane& operator=(const Plane& o) noexcept = default;

        inline const glm::vec3 getPoint() const { return this->point; }

        inline const glm::vec3 getNormal() const { return this->normal; }

        inline const bool collinearNormal(const glm::vec3& normal) const noexcept {
            const glm::vec3 sub = this->normal - normal;
            return (float)fabs(sub.x + sub.y + sub.z) < EPSILON;
        }

        inline const SIDE classifyPoint(const glm::vec3& point) const noexcept {
            const glm::vec3 dir = this->point - point;
            const float clipTest = glm::dot(dir, this->normal);

            if (fabs(clipTest) < EPSILON)
                return SIDE::CP_ONPLANE;

            if (clipTest < 0.0f)
                return SIDE::CP_FRONT;

            return SIDE::CP_BACK;
        }

        inline const SIDE classifyPoly(const glm::vec3& pA, const glm::vec3& pB, const glm::vec3& pC,
                                       glm::vec3& clipTest) const noexcept {
            uint8_t infront{0}, behind{0}, onPlane{0};

            clipTest.x = glm::dot((this->point - pA), this->normal); // Clip Test poin A
            clipTest.y = glm::dot((this->point - pB), this->normal); // Clip Test poin B
            clipTest.z = glm::dot((this->point - pC), this->normal); // Clip Test poin C

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

        inline const bool intersect(const glm::vec3& p0, const glm::vec3& p1, glm::vec3& intersection,
                                    float& percentage) const noexcept {

            const glm::vec3 direction = p1 - p0;
            const float linelength = glm::dot(direction, this->normal);
            if (fabsf(linelength) < 0.0001) // FIXME: EPISLON????
                return false;

            const glm::vec3 L1 = this->point - p0;
            const float dist_from_plane = glm::dot(L1, this->normal);
            percentage = dist_from_plane / linelength;

            if (percentage < 0.0f)
                return false;
            else if (percentage > 1.0f)
                return false;

            intersection = p0 + (direction * percentage);
            return true;
        }

        inline const bool AABBBehind(const std::array<glm::vec3, 8>& vList) const noexcept {
            return glm::dot(normal, vList[O]) < ND;
        }

        // FIXME: dot normal posicao para distancia (calcula do AABB em relacao ao frustum)
        // inline const float AABBDistance(const std::vector<glm::vec3>& vList) const noexcept { return glm::dot(normal,
        // vList[O]); }

      private:
        void calcND() noexcept {
            ND = dot(normal, point);
            O = normal.z < 0.0f ? (normal.y < 0.0f ? (normal.x < 0.0f ? 0 : 1) : (normal.x < 0.0f ? 2 : 3))
                                : (normal.y < 0.0f ? (normal.x < 0.0f ? 4 : 5) : (normal.x < 0.0f ? 6 : 7));
        }
    };
} // namespace ce
