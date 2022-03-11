#include "chimera/render/partition/space.hpp"
#include "chimera/render/OpenGLDefs.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

void spaceRenderAABB(const AABB& aabb) {

    const glm::vec3* vertex = aabb.getVertexs();

    glBegin(GL_LINES);

    glVertex3fv(glm::value_ptr(vertex[0]));
    glVertex3fv(glm::value_ptr(vertex[1]));
    glVertex3fv(glm::value_ptr(vertex[2]));
    glVertex3fv(glm::value_ptr(vertex[3]));
    glVertex3fv(glm::value_ptr(vertex[4]));
    glVertex3fv(glm::value_ptr(vertex[5]));
    glVertex3fv(glm::value_ptr(vertex[6]));
    glVertex3fv(glm::value_ptr(vertex[7]));

    glVertex3fv(glm::value_ptr(vertex[0]));
    glVertex3fv(glm::value_ptr(vertex[2]));
    glVertex3fv(glm::value_ptr(vertex[1]));
    glVertex3fv(glm::value_ptr(vertex[3]));
    glVertex3fv(glm::value_ptr(vertex[4]));
    glVertex3fv(glm::value_ptr(vertex[6]));
    glVertex3fv(glm::value_ptr(vertex[5]));
    glVertex3fv(glm::value_ptr(vertex[7]));

    glVertex3fv(glm::value_ptr(vertex[0]));
    glVertex3fv(glm::value_ptr(vertex[4]));
    glVertex3fv(glm::value_ptr(vertex[1]));
    glVertex3fv(glm::value_ptr(vertex[5]));
    glVertex3fv(glm::value_ptr(vertex[2]));
    glVertex3fv(glm::value_ptr(vertex[6]));
    glVertex3fv(glm::value_ptr(vertex[3]));
    glVertex3fv(glm::value_ptr(vertex[7]));

    glEnd();
}

void spaceRenderFrustum(const Frustum& frustum) {

    const glm::vec3* vertices = frustum.getVertexs();

    glBegin(GL_LINES);

    glVertex3fv(glm::value_ptr(vertices[0]));
    glVertex3fv(glm::value_ptr(vertices[1]));
    glVertex3fv(glm::value_ptr(vertices[2]));
    glVertex3fv(glm::value_ptr(vertices[3]));
    glVertex3fv(glm::value_ptr(vertices[4]));
    glVertex3fv(glm::value_ptr(vertices[5]));
    glVertex3fv(glm::value_ptr(vertices[6]));
    glVertex3fv(glm::value_ptr(vertices[7]));

    glVertex3fv(glm::value_ptr(vertices[0]));
    glVertex3fv(glm::value_ptr(vertices[2]));
    glVertex3fv(glm::value_ptr(vertices[1]));
    glVertex3fv(glm::value_ptr(vertices[3]));
    glVertex3fv(glm::value_ptr(vertices[4]));
    glVertex3fv(glm::value_ptr(vertices[6]));
    glVertex3fv(glm::value_ptr(vertices[5]));
    glVertex3fv(glm::value_ptr(vertices[7]));

    glVertex3fv(glm::value_ptr(vertices[0]));
    glVertex3fv(glm::value_ptr(vertices[4]));
    glVertex3fv(glm::value_ptr(vertices[1]));
    glVertex3fv(glm::value_ptr(vertices[5]));
    glVertex3fv(glm::value_ptr(vertices[2]));
    glVertex3fv(glm::value_ptr(vertices[6]));
    glVertex3fv(glm::value_ptr(vertices[3]));
    glVertex3fv(glm::value_ptr(vertices[7]));

    glEnd();
}

} // namespace Chimera