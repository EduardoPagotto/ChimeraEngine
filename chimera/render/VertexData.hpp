#ifndef __CHIMERA_VERTEX_HPP
#define __CHIMERA_VERTEX_HPP

#include <glm/glm.hpp>

// Tutorial opengl shadeGL - https://learnopengl.com/Getting-started/Shaders

#define EPSILON 1e-1
#define BUFFER_OFFSET(i) ((void*)(i))

namespace Chimera {

struct VertexData {
    glm::vec3 position; // 3 * 4 = 12 ( 0 - 11)
    glm::vec3 normal;   // 3 * 4 = 12 (12 - 23)
    glm::vec2 texture;  // 2 * 4 = 08 (24 - 31)
};
} // namespace Chimera
#endif