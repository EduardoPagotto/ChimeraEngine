#pragma once
#include "chimera/core/Mesh.hpp"
#include "chimera/core/space/Triangle.hpp"
#include <list>

// Tutorial opengl shadeGL - https://learnopengl.com/Getting-started/Shaders
namespace Chimera {

struct VertexData {
    glm::vec3 point;  // 3 * 4 = 12 ( 0 - 11)
    glm::vec3 normal; // 3 * 4 = 12 (12 - 23)
    glm::vec2 uv;     // 2 * 4 = 08 (24 - 31)
};

struct VertexDataSimple {
    glm::vec3 point; // 3 * 4 = 12 (0 - 11)
    glm::vec2 uv;    // 2 * 4 = 8  (12 - 19)
    float tid;       // 1 * 4 = 4  (20 - 23)
    glm::vec4 color; // 4 * 4 = 16 (24 - 39)
};

void vertexDataIndexCompile(std::vector<VertexData>& inData, std::vector<VertexData>& outData, std::vector<uint32_t>& out_indices);

void vertexDataReorder(std::vector<VertexData>& inVertexData, std::vector<uint32_t>& inIndex, std::vector<VertexData>& outVertexData,
                       std::vector<uint32_t>& outIndex);

// glm::vec3 aproxEpsilon(const glm::vec3& dado);
void vertexDataMinMaxSize(VertexData* pVertexList, const uint32_t& vertexSize, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void vertexDataIndexMinMaxSize(VertexData* pVertexList, const uint32_t vertexSize, uint32_t* pIndexList, const uint32_t indexSize,
                               glm::vec3& min, glm::vec3& max, glm::vec3& size);

void vertexDataMeshDebug(Mesh* pM, bool _showAll);
void vertexDataMeshMinMaxSize(Mesh* m, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void vertexDataFromMesh(Mesh* m, std::vector<VertexData>& outData);
void vertexDataMeshScale(Mesh* m, const float& new_size, const bool& hasTexture);
void vertexDataMeshClean(Mesh* m);

void vertexDataIndexToTriangle(VertexData* vertexData, uint32_t* indexData, const uint32_t& indexSize, std::list<Triangle*>& vTris);
void vertexDataToTriangle(VertexData* vertexData, const uint32_t& vertexSize, std::list<Triangle*>& vTris);
} // namespace Chimera
