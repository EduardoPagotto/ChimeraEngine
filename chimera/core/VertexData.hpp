#ifndef __CHIMERA_VERTEX_HPP
#define __CHIMERA_VERTEX_HPP

#include <glm/glm.hpp>
#include <vector>

// Tutorial opengl shadeGL - https://learnopengl.com/Getting-started/Shaders
#define BUFFER_OFFSET(i) ((void*)(i))

#define EPSILON 1e-2
//#define EPSILON 1e-1 // Falhas em calculos de aproximação verificar projeto bsptree

enum class SIDE { CP_ONPLANE = 0, CP_FRONT, CP_BACK, CP_SPANNING };

namespace Chimera {

struct VertexData {
    glm::vec3 position; // 3 * 4 = 12 ( 0 - 11)
    glm::vec3 normal;   // 3 * 4 = 12 (12 - 23)
    glm::vec2 texture;  // 2 * 4 = 08 (24 - 31)
};

struct MeshData {
    bool singleIndex;
    std::vector<glm::vec3> vertexList;
    std::vector<glm::vec3> normalList;
    std::vector<glm::vec2> uvList;
    std::vector<uint32_t> vertexIndex;
    std::vector<uint32_t> normalIndex;
    std::vector<uint32_t> uvIndex;
};

struct VertexComponents {
    struct __Loc {
        uint8_t id;
        uint8_t size;
        uint16_t type;
        uint16_t normalize;
        void* offset;
    };
    std::vector<__Loc> loc;
    uint32_t sizeVertex;
};

void vertexDataIndexCompile(std::vector<VertexData>& inData, std::vector<VertexData>& outData, std::vector<uint32_t>& out_indices);

void vertexDataReorder(std::vector<VertexData>& inVertexData, std::vector<uint32_t>& inIndex, std::vector<VertexData>& outVertexData,
                       std::vector<uint32_t>& outIndex);

// glm::vec3 aproxEpsilon(const glm::vec3& dado);
void vertexDataMinMaxSize(VertexData* pVertexList, const uint32_t& vertexSize, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void vertexDataIndexMinMaxSize(VertexData* pVertexList, const uint32_t vertexSize, uint32_t* pIndexList, const uint32_t indexSize,
                               glm::vec3& min, glm::vec3& max, glm::vec3& size);

void vertexDataMeshDataDebug(MeshData* pM, bool _showAll);
void vertexDataMeshMinMaxSize(MeshData* m, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void vertexDataFromMesh(MeshData* m, std::vector<VertexData>& outData);
void vertexDataMeshScale(MeshData* m, const float& new_size, const bool& hasTexture);
void vertexDataMeshClean(MeshData* m);
VertexComponents* vertexDataComponentes();
VertexComponents* vertexDataFloatComponentes();
VertexComponents* vertexDataSingleComponentes(uint8_t id, uint8_t sizeSlot, uint8_t sizeVetex);
} // namespace Chimera
#endif