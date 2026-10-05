#pragma once
#include "space.hpp"
#include <array>

namespace ce {

    enum class SIDE { CP_ONPLANE, CP_FRONT, CP_BACK, CP_SPANNING };

    class Plane {
      public:
        explicit Plane() noexcept = default;
        explicit Plane(const Plane& o) noexcept = default;
        explicit Plane(const glm::vec3& point, const glm::vec3& normal) noexcept : point_(point), normal_(normal) {
            this->calc_nd();
        }

        explicit Plane(const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc) noexcept : point_(pa) {
            normal_ = glm::normalize(glm::cross(pb - pa, pc - pa));
            this->calc_nd();
        }

        virtual ~Plane() noexcept = default;

        Plane& operator=(const Plane& o) noexcept = default;

        glm::vec3 get_point() const { return this->point_; }
        glm::vec3 get_normal() const { return this->normal_; }

        bool collinear_normal(const glm::vec3& normal) const noexcept {
            const glm::vec3 sub = this->normal_ - normal;
            return is_less_epsilon(sub.x + sub.y + sub.z);
        }

        SIDE classify_point(const glm::vec3& point) const noexcept {
            const glm::vec3 dir = this->point_ - point;
            const float clip_test = glm::dot(dir, this->normal_);

            if (is_less_epsilon(clip_test)) {
                return SIDE::CP_ONPLANE;
            }

            if (clip_test < 0.0F) {
                return SIDE::CP_FRONT;
            }

            return SIDE::CP_BACK;
        }

        SIDE classify_poly(const glm::vec3& p_a, const glm::vec3& p_b, const glm::vec3& p_c,
                           glm::vec3& clip_test) const noexcept {

            uint8_t infront{0};
            uint8_t behind{0};
            uint8_t on_plane{0};

            clip_test.x = glm::dot((this->point_ - p_a), this->normal_); // Clip Test poin A
            clip_test.y = glm::dot((this->point_ - p_b), this->normal_); // Clip Test poin B
            clip_test.z = glm::dot((this->point_ - p_c), this->normal_); // Clip Test poin C

            for (uint8_t i = 0; i < 3; i++) {
                if (is_less_epsilon(clip_test[i])) {
                    clip_test[i] = 0.0F;
                    on_plane++;
                    infront++;
                    behind++;
                } else if (clip_test[i] > 0.0F) {
                    behind++;
                } else { // clipTest[i] < 0.0F
                    infront++;
                }
            }

            if (on_plane == 3) {
                return SIDE::CP_ONPLANE;
            }

            if (behind == 3) {
                return SIDE::CP_BACK;
            }

            if (infront == 3) {
                return SIDE::CP_FRONT;
            }

            return SIDE::CP_SPANNING;
        }

        bool intersect(const glm::vec3& p0, const glm::vec3& p1, glm::vec3& intersection,
                       float& percentage) const noexcept {

            const glm::vec3 direction = p1 - p0;
            const float linelength = glm::dot(direction, this->normal_);
            if (fabsf(linelength) < 0.0001) { // FIXME: EPISLON????
                return false;
            }

            const glm::vec3 l1 = this->point_ - p0;
            const float dist_from_plane = glm::dot(l1, this->normal_);
            percentage = dist_from_plane / linelength;

            if (percentage < 0.0F) {
                return false;
            }

            if (percentage > 1.0F) {
                return false;
            }

            intersection = p0 + (direction * percentage);
            return true;
        }

        bool aabb_behind(const std::array<glm::vec3, 8>& v_list) const noexcept {
            return glm::dot(normal_, v_list[o_]) < nd_;
        }

        // FIXME: dot normal posicao para distancia (calcula do AABB em relacao ao frustum)
        // inline const float AABBDistance(const std::vector<glm::vec3>& vList) const noexcept { return glm::dot(normal,
        // vList[O]); }

      private:
        void calc_nd() noexcept {
            nd_ = dot(normal_, point_);
            o_ = normal_.z < 0.0F ? (normal_.y < 0.0F ? (normal_.x < 0.0F ? 0 : 1) : (normal_.x < 0.0F ? 2 : 3))
                                  : (normal_.y < 0.0F ? (normal_.x < 0.0F ? 4 : 5) : (normal_.x < 0.0F ? 6 : 7));
        }

        glm::vec3 point_{0.0F};  // vertice A
        glm::vec3 normal_{0.0F}; // plane calc cross product B and C across A
        float nd_{0.0F};
        int o_{0};
    };
} // namespace ce
