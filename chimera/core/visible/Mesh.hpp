#pragma once
#include "chimera/core/TagComponent.hpp"
#include "chimera/core/space/Triangle.hpp"
#include "chimera/core/space/TrisIndex.hpp"
#include <glm/glm.hpp>
#include <list>

namespace Chimera {
struct Mesh {
    Mesh() = default;
    bool serialized = false;
    std::vector<glm::vec3> point;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec2> uv;
    std::vector<uint32_t> iPoint;
    std::vector<uint32_t> iNormal;
    std::vector<uint32_t> iUv;
};

enum class MeshType { SIMPLE = 0, ARRAY = 1, BSTREE = 2 };

struct MeshComponent {
    TagComponent tag;
    MeshType type = MeshType::SIMPLE;
    Mesh* mesh = new Mesh();
    std::vector<TrisIndex> vTrisIndex;
    MeshComponent() = default;
};

void meshDataClean(Mesh* m);
void meshMinMaxSize(Mesh* m, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void meshDataScale(Mesh* m, const float& new_size, const bool& hasTexture);
void meshToTriangle(Mesh* m, std::list<Triangle*>& vTris);
void meshReCompile(Mesh& inData, Mesh& outData);
void meshDebug(Mesh* m, bool _showAll);

} // namespace Chimera