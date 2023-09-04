#pragma once
#include "chimera/core/TagComponent.hpp"
#include "chimera/core/space/Triangle.hpp"
#include <glm/glm.hpp>
#include <list>

namespace Chimera {

struct VertexData {
    glm::vec3 point;  // 3 * 4 = 12 ( 0 - 11)
    glm::vec3 normal; // 3 * 4 = 12 (12 - 23)
    glm::vec2 uv;     // 2 * 4 = 08 (24 - 31)
};

struct Mesh {
    Mesh() = default;
    std::vector<VertexData> vertex;
    TrisIndex iFace;
};

enum class MeshType { SIMPLE = 0, ARRAY = 1, BSTREE = 2 };

struct MeshComponent {
    TagComponent tag;
    MeshType type = MeshType::SIMPLE;
    Mesh* mesh = new Mesh();
    std::vector<TrisIndex> vTrisIndex;
    MeshComponent() = default;
};

MeshType getMeshTypeFromString(const std::string& text);
void meshDataClean(Mesh* m);
void meshMinMaxSize(Mesh* m, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void meshDataScale(Mesh* m, const float& new_size, const bool& hasTexture);
void meshToTriangle(Mesh* m, std::list<Triangle*>& vTris);
void meshReCompile(Mesh& inData, Mesh& outData);
void meshDebug(Mesh* m, bool _showAll);
void meshSerialize(Mesh& inData, Mesh& outData);

void idxSimplifieVec3(std::vector<glm::vec3>& in, std::vector<glm::vec3>& out, std::vector<uint32_t>& idxIn, std::vector<uint32_t>& idxOut);
void idxSimplifieVec2(std::vector<glm::vec2>& in, std::vector<glm::vec2>& out, std::vector<uint32_t>& idxIn, std::vector<uint32_t>& idxOut);
} // namespace Chimera