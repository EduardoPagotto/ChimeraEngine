#include "chimera/render/AABB.hpp"
#include "chimera/OpenGLDefs.hpp"
#include <glm/gtc/type_ptr.hpp>

// ref: http://www.3dcpptutorials.sk/index.php?id=59

namespace chimera {

AABB::AABB() {}

AABB::~AABB() {}

void AABB::set(const glm::vec3& _min, const glm::vec3& _max) {
    vertices[0] = glm::vec3(_min.x, _min.y, _min.z);
    vertices[1] = glm::vec3(_max.x, _min.y, _min.z);
    vertices[2] = glm::vec3(_min.x, _max.y, _min.z);
    vertices[3] = glm::vec3(_max.x, _max.y, _min.z);
    vertices[4] = glm::vec3(_min.x, _min.y, _max.z);
    vertices[5] = glm::vec3(_max.x, _min.y, _max.z);
    vertices[6] = glm::vec3(_min.x, _max.y, _max.z);
    vertices[7] = glm::vec3(_max.x, _max.y, _max.z);
}

bool AABB::pointInside(const glm::vec3& _point) {
    if (_point.x < vertices[0].x)
        return false;
    if (_point.y < vertices[0].y)
        return false;
    if (_point.z < vertices[0].z)
        return false;

    if (_point.x > vertices[7].x)
        return false;
    if (_point.y > vertices[7].y)
        return false;
    if (_point.z > vertices[7].z)
        return false;

    return true;
}

// bool AABB::Visible(CFrustum& Frustum) { return Frustum.AABBVisible(vertices); }

// float AABB::Distance(CFrustum& Frustum) { return Frustum.AABBDistance(vertices); }

void AABB::render() {
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

} // namespace chimera