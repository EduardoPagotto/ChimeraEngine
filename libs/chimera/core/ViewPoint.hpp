#ifndef __CHIMERA_VIEW_POINT__HPP
#define __CHIMERA_VIEW_POINT__HPP

#include <glm/glm.hpp>

namespace Chimera {

struct ViewPoint {

    ViewPoint() { this->zero(); }

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 direction;

    float near;
    float far;
    float fov;

    void zero() {
        position = glm::vec3(0.0, 0.0, 0.0);
        rotation = glm::vec3(0.0, 0.0, -1.0);
        direction = glm::vec3(0.0, 0.0, 0.0);
        near = 0.1f;
        far = 2000.0f;
        fov = 45.0f;
    }

    void setTransform(const glm::mat4& _trans) {
        position = glm::vec3(_trans[3]);
        direction = glm::vec3(0.0, 0.0, 0.0); // FIXME encontrar na matrix _trans
        rotation = glm::vec3(0.0, 0.0, -1.0); // FIXME pegar no arquivo do collada
    }
};
} // namespace Chimera
#endif