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

void vertexDataIndexCompile(std::vector<VertexData>& inData, std::vector<VertexData>& outData, std::vector<uint32_t>& out_indices);
void vertexDataMinMaxSize(VertexData* pVertexList, const uint32_t& vertexSize, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void vertexDataIndexMinMaxSize(VertexData* pVertexList, const uint32_t vertexSize, uint32_t* pIndexList, const uint32_t indexSize,
                               glm::vec3& min, glm::vec3& max, glm::vec3& size);

void vertexDataFromMesh(Mesh* m, std::vector<VertexData>& outData);
} // namespace Chimera
