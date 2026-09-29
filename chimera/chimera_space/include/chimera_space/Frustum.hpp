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
            const glm::vec4 A = vpmi * glm::vec4(-1.0F, -1.0F, 1.0F, 1.0F);  // 4
            const glm::vec4 B = vpmi * glm::vec4(1.0F, -1.0F, 1.0F, 1.0F);   // 5
            const glm::vec4 C = vpmi * glm::vec4(-1.0F, 1.0F, 1.0F, 1.0F);   // 6
            const glm::vec4 D = vpmi * glm::vec4(1.0F, 1.0F, 1.0F, 1.0F);    // 7
            const glm::vec4 E = vpmi * glm::vec4(-1.0F, -1.0F, -1.0F, 1.0F); // 0
            const glm::vec4 F = vpmi * glm::vec4(1.0F, -1.0F, -1.0F, 1.0F);  // 1
            const glm::vec4 G = vpmi * glm::vec4(-1.0F, 1.0F, -1.0F, 1.0F);  // 2
            const glm::vec4 H = vpmi * glm::vec4(1.0F, 1.0F, -1.0F, 1.0F);   // 3

            points_[0] = glm::vec3(A.x / A.w, A.y / A.w, A.z / A.w);
            points_[1] = glm::vec3(B.x / B.w, B.y / B.w, B.z / B.w);
            points_[2] = glm::vec3(C.x / C.w, C.y / C.w, C.z / C.w);
            points_[3] = glm::vec3(D.x / D.w, D.y / D.w, D.z / D.w);
            points_[4] = glm::vec3(E.x / E.w, E.y / E.w, E.z / E.w);
            points_[5] = glm::vec3(F.x / F.w, F.y / F.w, F.z / F.w);
            points_[6] = glm::vec3(G.x / G.w, G.y / G.w, G.z / G.w);
            points_[7] = glm::vec3(H.x / H.w, H.y / H.w, H.z / H.w);

            planes_[0] = Plane(points_[4], points_[0], points_[2]);
            planes_[1] = Plane(points_[1], points_[5], points_[7]);
            planes_[2] = Plane(points_[4], points_[5], points_[1]);
            planes_[3] = Plane(points_[2], points_[3], points_[7]);
            planes_[4] = Plane(points_[0], points_[1], points_[3]);
            planes_[5] = Plane(points_[5], points_[4], points_[6]);
        }

        bool aabbVisible(const std::array<glm::vec3, 8>& vList) const {
            return std::ranges::all_of(planes_, [&vList](const Plane& plane) { return !plane.aabbBehind(vList); });
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
