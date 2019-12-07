#include "chimera/render/Triangle.hpp"

namespace Chimera {

unsigned Triangle::serialMaster = 0;

Triangle::Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) : serial(++serialMaster) {
    this->vertex[0].position = a;
    this->vertex[1].position = b;
    this->vertex[2].position = c;
}

Triangle::Triangle(const Triangle& _cpy) {
    serial = _cpy.serial;
    for (int i = 0; i < 3; i++) {
        this->vertex[i].position = _cpy.vertex[i].position;
        this->vertex[i].normal = _cpy.vertex[i].normal;
        this->vertex[i].texture = _cpy.vertex[i].texture;
    }
}

glm::vec3 Triangle::normal() {
    glm::vec3 acc = glm::vec3(0, 0, 0);
    for (int i = 0; i < 3; i++)
        acc = acc + this->vertex[i].normal;
    return glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
}

// void Triangle::debugData() {
//     for (int i = 0; i < 3; i++) {
//         if (i == 0) {
//             SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "---------------------------------------------------");
//             SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "A");
//         } else if (i == 1)
//             SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "B");
//         else
//             SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "C");

//         SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "vertex (%f %f %f)", this->vertex[i].position.x,
//                      this->vertex[i].position.y, this->vertex[i].position.z);

//         SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "normal (%f %f %f)", this->vertex[i].normal.x,
//                      this->vertex[i].normal.y, this->vertex[i].normal.z);

//         SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "texture (%f %f)", this->vertex[i].texture.x,
//                      this->vertex[i].texture.y);
//     }
// }

} // namespace Chimera
// glm::vec3 Triangle::verticeNormal() {
//     glm::vec3 u = this->vertex[0].position - this->vertex[2].position;
//     glm::vec3 v = this->vertex[0].position - this->vertex[1].position;
//     glm::vec3 normal = glm::normalize(glm::cross(u, v));
//     for (int i = 0; i < 3; i++) {
//         this->vertex[i].normal = normal;
//     }
//     return normal;
// }
