#ifndef __CHIMERA_VIEW_POINT__HPP
#define __CHIMERA_VIEW_POINT__HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

class ViewPoint {
  public:
    ViewPoint() { this->zero(); }

    ViewPoint(const ViewPoint& _copy) {
        position = _copy.position;
        front = _copy.front;
        up = _copy.up;
        near = _copy.near;
        far = _copy.far;
        fov = _copy.fov;
    }

    void zero() {
        position = glm::vec3(0.0, 0.0, 0.0);
        front = glm::vec3(0.0, 0.0, -1.0);
        up = glm::vec3(0.0, 1.0, 0.0);
        worldUp = up;
        near = 0.1f;
        far = 2000.0f;
        fov = 45.0f;
    }

    void zoom(float _zoonOffset) {
        if (fov >= 1.0f && fov <= 45.0f)
            fov -= _zoonOffset;
        if (fov <= 1.0f)
            fov = 1.0f;
        if (fov >= 45.0f)
            fov = 45.0f;
    }

    void setTransform(const glm::mat4& _trans) {
        // FIXME: encontrar na matrix _trans
        position = glm::vec3(_trans[3]);
        front = glm::vec3(0.0, 0.0, -1.0);
        up = glm::vec3(0.0, 1.0, 0.0);
        worldUp = up;
    }

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 worldUp;
    float near;
    float far;
    float fov;
};
} // namespace Chimera
#endif
