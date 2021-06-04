#ifndef __CHIMERA_VERTEX_HPP
#define __CHIMERA_VERTEX_HPP

#include <glm/glm.hpp>
#include <vector>

// Tutorial opengl shadeGL - https://learnopengl.com/Getting-started/Shaders
#define BUFFER_OFFSET(i) ((void*)(i))

namespace Chimera {

struct VertexData {
    glm::vec3 position; // 3 * 4 = 12 ( 0 - 11)
    glm::vec3 normal;   // 3 * 4 = 12 (12 - 23)
    glm::vec2 texture;  // 2 * 4 = 08 (24 - 31)
};

void vertexDataIndexCompile(std::vector<VertexData>& inData, std::vector<VertexData>& outData, std::vector<unsigned int>& out_indices);
// glm::vec3 aproxEpsilon(const glm::vec3& dado);
void vertexDataMinMaxSize(std::vector<VertexData>& vertexList, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void vertexDataIndexMinMaxSize(std::vector<VertexData>& vertexList, std::vector<uint32_t>& indexList, glm::vec3& min, glm::vec3& max,
                               glm::vec3& size);

} // namespace Chimera
#endif