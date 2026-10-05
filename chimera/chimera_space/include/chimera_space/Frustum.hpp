#pragma once
#include "Plane.hpp"
#include <algorithm>
#include <array>

namespace ce {

    class Frustum {
      public:
        explicit Frustum() noexcept = default;
        explicit Frustum(const Frustum& o) = delete;
        Frustum& operator=(Frustum& o) = delete;
        virtual ~Frustum() noexcept = default;

        void set(const glm::mat4& vpmi) noexcept {                           // ViewProjectionMatrixInverse
            const glm::vec4 a = vpmi * glm::vec4(-1.0F, -1.0F, 1.0F, 1.0F);  // 4
            const glm::vec4 b = vpmi * glm::vec4(1.0F, -1.0F, 1.0F, 1.0F);   // 5
            const glm::vec4 c = vpmi * glm::vec4(-1.0F, 1.0F, 1.0F, 1.0F);   // 6
            const glm::vec4 d = vpmi * glm::vec4(1.0F, 1.0F, 1.0F, 1.0F);    // 7
            const glm::vec4 e = vpmi * glm::vec4(-1.0F, -1.0F, -1.0F, 1.0F); // 0
            const glm::vec4 f = vpmi * glm::vec4(1.0F, -1.0F, -1.0F, 1.0F);  // 1
            const glm::vec4 g = vpmi * glm::vec4(-1.0F, 1.0F, -1.0F, 1.0F);  // 2
            const glm::vec4 h = vpmi * glm::vec4(1.0F, 1.0F, -1.0F, 1.0F);   // 3

            points_[0] = glm::vec3(a.x / a.w, a.y / a.w, a.z / a.w);
            points_[1] = glm::vec3(b.x / b.w, b.y / b.w, b.z / b.w);
            points_[2] = glm::vec3(c.x / c.w, c.y / c.w, c.z / c.w);
            points_[3] = glm::vec3(d.x / d.w, d.y / d.w, d.z / d.w);
            points_[4] = glm::vec3(e.x / e.w, e.y / e.w, e.z / e.w);
            points_[5] = glm::vec3(f.x / f.w, f.y / f.w, f.z / f.w);
            points_[6] = glm::vec3(g.x / g.w, g.y / g.w, g.z / g.w);
            points_[7] = glm::vec3(h.x / h.w, h.y / h.w, h.z / h.w);

            planes_[0] = Plane(points_[4], points_[0], points_[2]);
            planes_[1] = Plane(points_[1], points_[5], points_[7]);
            planes_[2] = Plane(points_[4], points_[5], points_[1]);
            planes_[3] = Plane(points_[2], points_[3], points_[7]);
            planes_[4] = Plane(points_[0], points_[1], points_[3]);
            planes_[5] = Plane(points_[5], points_[4], points_[6]);
        }

        bool aabb_visible(const std::array<glm::vec3, 8>& v_list) const {
            return std::ranges::all_of(planes_, [&v_list](const Plane& plane) { return !plane.aabb_behind(v_list); });
        }

        // FIXME: dot normal posicao para distancia (calcula do AABB em relacao ao frustum)
        // float aabbDistance(const std::array<glm::vec3, 8>& vList) const {
        //      return planes[5].aabbDistance(vList);
        // }

        // void render_debug() const {
        //     glBegin(GL_LINES);

        //     glVertex3fv(glm::value_ptr(points[0]));
        //     glVertex3fv(glm::value_ptr(points[1]));
        //     glVertex3fv(glm::value_ptr(points[2]));
        //     glVertex3fv(glm::value_ptr(points[3]));
        //     glVertex3fv(glm::value_ptr(points[4]));
        //     glVertex3fv(glm::value_ptr(points[5]));
        //     glVertex3fv(glm::value_ptr(points[6]));
        //     glVertex3fv(glm::value_ptr(points[7]));
        //     glVertex3fv(glm::value_ptr(points[0]));
        //     glVertex3fv(glm::value_ptr(points[2]));
        //     glVertex3fv(glm::value_ptr(points[1]));
        //     glVertex3fv(glm::value_ptr(points[3]));
        //     glVertex3fv(glm::value_ptr(points[4]));
        //     glVertex3fv(glm::value_ptr(points[6]));
        //     glVertex3fv(glm::value_ptr(points[5]));
        //     glVertex3fv(glm::value_ptr(points[7]));

        //     glVertex3fv(glm::value_ptr(points[0]));
        //     glVertex3fv(glm::value_ptr(points[4]));
        //     glVertex3fv(glm::value_ptr(points[1]));
        //     glVertex3fv(glm::value_ptr(points[5]));
        //     glVertex3fv(glm::value_ptr(points[2]));
        //     glVertex3fv(glm::value_ptr(points[6]));
        //     glVertex3fv(glm::value_ptr(points[3]));
        //     glVertex3fv(glm::value_ptr(points[7]));

        //     glEnd();
        // }
      private:
        std::array<glm::vec3, 8> points_{};
        std::array<Plane, 6> planes_;
    };
} // namespace ce
