#pragma once
#include "Plane.hpp"
#include <array>

namespace ce {

    class Frustum {
      private:
        std::array<glm::vec3, 8> points{};
        std::array<Plane, 6> planes;

      public:
        explicit Frustum() noexcept = default;
        explicit Frustum(const Frustum& o) = delete;
        Frustum& operator=(Frustum& o) = delete;
        virtual ~Frustum() noexcept = default;

        void set(const glm::mat4& vpmi) noexcept {                           // ViewProjectionMatrixInverse
            const glm::vec4 A = vpmi * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);  // 4
            const glm::vec4 B = vpmi * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);   // 5
            const glm::vec4 C = vpmi * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);   // 6
            const glm::vec4 D = vpmi * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    // 7
            const glm::vec4 E = vpmi * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f); // 0
            const glm::vec4 F = vpmi * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);  // 1
            const glm::vec4 G = vpmi * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);  // 2
            const glm::vec4 H = vpmi * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);   // 3

            points[0] = glm::vec3(A.x / A.w, A.y / A.w, A.z / A.w);
            points[1] = glm::vec3(B.x / B.w, B.y / B.w, B.z / B.w);
            points[2] = glm::vec3(C.x / C.w, C.y / C.w, C.z / C.w);
            points[3] = glm::vec3(D.x / D.w, D.y / D.w, D.z / D.w);
            points[4] = glm::vec3(E.x / E.w, E.y / E.w, E.z / E.w);
            points[5] = glm::vec3(F.x / F.w, F.y / F.w, F.z / F.w);
            points[6] = glm::vec3(G.x / G.w, G.y / G.w, G.z / G.w);
            points[7] = glm::vec3(H.x / H.w, H.y / H.w, H.z / H.w);

            planes[0] = Plane(points[4], points[0], points[2]);
            planes[1] = Plane(points[1], points[5], points[7]);
            planes[2] = Plane(points[4], points[5], points[1]);
            planes[3] = Plane(points[2], points[3], points[7]);
            planes[4] = Plane(points[0], points[1], points[3]);
            planes[5] = Plane(points[5], points[4], points[6]);
        }

        const bool AABBVisible(const std::array<glm::vec3, 8>& vList) const {

            for (const Plane& plane : planes) {
                if (plane.AABBBehind(vList)) {
                    return false;
                }
            }
            return true;
        }

        // FIXME: dot normal posicao para distancia (calcula do AABB em relacao ao frustum)
        // inline const float AABBDistance(const std::vector<glm::vec3>& vList) const { return
        // planes[5].AABBDistance(vList); }

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
    };
} // namespace ce
