#include "chimera/core/space/Frustum.hpp"

namespace Chimera {

Frustum::Frustum() {}

Frustum::~Frustum() {}

void Frustum::set(const glm::mat4& ViewProjectionMatrixInverse) {

    glm::vec4 A = ViewProjectionMatrixInverse * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
    glm::vec4 B = ViewProjectionMatrixInverse * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
    glm::vec4 C = ViewProjectionMatrixInverse * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 D = ViewProjectionMatrixInverse * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 E = ViewProjectionMatrixInverse * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
    glm::vec4 F = ViewProjectionMatrixInverse * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);
    glm::vec4 G = ViewProjectionMatrixInverse * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
    glm::vec4 H = ViewProjectionMatrixInverse * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);

    vertices[0] = glm::vec3(A.x / A.w, A.y / A.w, A.z / A.w);
    vertices[1] = glm::vec3(B.x / B.w, B.y / B.w, B.z / B.w);
    vertices[2] = glm::vec3(C.x / C.w, C.y / C.w, C.z / C.w);
    vertices[3] = glm::vec3(D.x / D.w, D.y / D.w, D.z / D.w);
    vertices[4] = glm::vec3(E.x / E.w, E.y / E.w, E.z / E.w);
    vertices[5] = glm::vec3(F.x / F.w, F.y / F.w, F.z / F.w);
    vertices[6] = glm::vec3(G.x / G.w, G.y / G.w, G.z / G.w);
    vertices[7] = glm::vec3(H.x / H.w, H.y / H.w, H.z / H.w);

    planes[0].set(vertices[4], vertices[0], vertices[2]);
    planes[1].set(vertices[1], vertices[5], vertices[7]);
    planes[2].set(vertices[4], vertices[5], vertices[1]);
    planes[3].set(vertices[2], vertices[3], vertices[7]);
    planes[4].set(vertices[0], vertices[1], vertices[3]);
    planes[5].set(vertices[5], vertices[4], vertices[6]);
}

bool Frustum::AABBVisible(const glm::vec3* AABBVertices) const {
    for (int i = 0; i < 6; i++) {
        if (planes[i].AABBBehind(AABBVertices)) {
            return false;
        }
    }

    return true;
}

float Frustum::AABBDistance(const glm::vec3* AABBVertices) const { return planes[5].AABBDistance(AABBVertices); }

} // namespace Chimera