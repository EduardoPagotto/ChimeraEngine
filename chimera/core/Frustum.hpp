#pragma once
#include "Plane.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

class Frustum {
  private:
    glm::vec3 m_vertices[8];
    Plane m_planes[6];

  public:
    Frustum() = default;
    virtual ~Frustum() = default;

    void set(const glm::mat4& ViewProjectionMatrixInverse) {
        const glm::vec4 A = ViewProjectionMatrixInverse * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
        const glm::vec4 B = ViewProjectionMatrixInverse * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
        const glm::vec4 C = ViewProjectionMatrixInverse * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
        const glm::vec4 D = ViewProjectionMatrixInverse * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        const glm::vec4 E = ViewProjectionMatrixInverse * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
        const glm::vec4 F = ViewProjectionMatrixInverse * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);
        const glm::vec4 G = ViewProjectionMatrixInverse * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
        const glm::vec4 H = ViewProjectionMatrixInverse * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);

        m_vertices[0] = glm::vec3(A.x / A.w, A.y / A.w, A.z / A.w);
        m_vertices[1] = glm::vec3(B.x / B.w, B.y / B.w, B.z / B.w);
        m_vertices[2] = glm::vec3(C.x / C.w, C.y / C.w, C.z / C.w);
        m_vertices[3] = glm::vec3(D.x / D.w, D.y / D.w, D.z / D.w);
        m_vertices[4] = glm::vec3(E.x / E.w, E.y / E.w, E.z / E.w);
        m_vertices[5] = glm::vec3(F.x / F.w, F.y / F.w, F.z / F.w);
        m_vertices[6] = glm::vec3(G.x / G.w, G.y / G.w, G.z / G.w);
        m_vertices[7] = glm::vec3(H.x / H.w, H.y / H.w, H.z / H.w);

        m_planes[0].set(m_vertices[4], m_vertices[0], m_vertices[2]);
        m_planes[1].set(m_vertices[1], m_vertices[5], m_vertices[7]);
        m_planes[2].set(m_vertices[4], m_vertices[5], m_vertices[1]);
        m_planes[3].set(m_vertices[2], m_vertices[3], m_vertices[7]);
        m_planes[4].set(m_vertices[0], m_vertices[1], m_vertices[3]);
        m_planes[5].set(m_vertices[5], m_vertices[4], m_vertices[6]);
    }

    const bool AABBVisible(const glm::vec3* AABBVertices) const {

        for (uint8_t i = 0; i < 6; i++) {
            if (m_planes[i].AABBBehind(AABBVertices)) {
                return false;
            }
        }

        return true;
    }

    inline const float AABBDistance(const glm::vec3* AABBVertices) const {
        // FIXME: porque 5 ????
        return m_planes[5].AABBDistance(AABBVertices);
    }

    void render_debug() const {
        // TODO: implementar usando vbo
        //  glBegin(GL_LINES);

        // glVertex3fv(glm::value_ptr(m_vertices[0]));
        // glVertex3fv(glm::value_ptr(m_vertices[1]));
        // glVertex3fv(glm::value_ptr(m_vertices[2]));
        // glVertex3fv(glm::value_ptr(m_vertices[3]));
        // glVertex3fv(glm::value_ptr(m_vertices[4]));
        // glVertex3fv(glm::value_ptr(m_vertices[5]));
        // glVertex3fv(glm::value_ptr(m_vertices[6]));
        // glVertex3fv(glm::value_ptr(m_vertices[7]));

        // glVertex3fv(glm::value_ptr(m_vertices[0]));
        // glVertex3fv(glm::value_ptr(m_vertices[2]));
        // glVertex3fv(glm::value_ptr(m_vertices[1]));
        // glVertex3fv(glm::value_ptr(m_vertices[3]));
        // glVertex3fv(glm::value_ptr(m_vertices[4]));
        // glVertex3fv(glm::value_ptr(m_vertices[6]));
        // glVertex3fv(glm::value_ptr(m_vertices[5]));
        // glVertex3fv(glm::value_ptr(m_vertices[7]));

        // glVertex3fv(glm::value_ptr(m_vertices[0]));
        // glVertex3fv(glm::value_ptr(m_vertices[4]));
        // glVertex3fv(glm::value_ptr(m_vertices[1]));
        // glVertex3fv(glm::value_ptr(m_vertices[5]));
        // glVertex3fv(glm::value_ptr(m_vertices[2]));
        // glVertex3fv(glm::value_ptr(m_vertices[6]));
        // glVertex3fv(glm::value_ptr(m_vertices[3]));
        // glVertex3fv(glm::value_ptr(m_vertices[7]));

        // glEnd();
    }
};
} // namespace Chimera
