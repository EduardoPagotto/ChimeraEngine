#ifndef __CHIMERA_VERTEX_HPP
#define __CHIMERA_VERTEX_HPP

#include <glm/glm.hpp>
#include <vector>

// Tutorial opengl shadeGL - https://learnopengl.com/Getting-started/Shaders

#define EPSILON 1e-2
//#define EPSILON 1e-1 // Falhas em calculos de aproximação verificar projeto bsptree

#define BUFFER_OFFSET(i) ((void*)(i))

namespace Chimera {

struct VertexData {
    glm::vec3 position; // 3 * 4 = 12 ( 0 - 11)
    glm::vec3 normal;   // 3 * 4 = 12 (12 - 23)
    glm::vec2 texture;  // 2 * 4 = 08 (24 - 31)
};

void vertexDataIndexCompile(std::vector<VertexData>& inData, std::vector<VertexData>& outData, std::vector<unsigned int>& out_indices);

} // namespace Chimera
#endif