#pragma once
#include "chimera/core/visible/Mesh.hpp"

// Tutorial opengl shadeGL - https://learnopengl.com/Getting-started/Shaders
namespace Chimera {

struct VertexDataSimple {
    glm::vec3 point; // 3 * 4 = 12 (0 - 11)
    glm::vec2 uv;    // 2 * 4 = 8  (12 - 19)
    float tid;       // 1 * 4 = 4  (20 - 23)
    glm::vec4 color; // 4 * 4 = 16 (24 - 39)
};

std::tuple<glm::vec3, glm::vec3, glm::vec3> vertexBoundaries(std::vector<VertexData>& vArray);
std::tuple<glm::vec3, glm::vec3, glm::vec3> vertexIndexedBoundaries(std::vector<VertexData>& vArray, TrisIndex& tris);
} // namespace Chimera
