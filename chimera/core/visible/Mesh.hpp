#pragma once
#include "chimera/core/TagComponent.hpp"
#include "chimera/core/space/Triangle.hpp"
#include <glm/glm.hpp>
#include <list>

namespace Chimera {

// enum class FaceIndexType {
//     SEQUENTIAL, // index starts 0 to size point = size normal = size uv (faces array empty)
//     UNIQUE,     // index of point/normal/Tex same to all (faces array 0 same to all)
//     MULTI,      // index 0 to point, 1 to normal, 2 to UV (faces[0] iPoint; faces[1] iNormal; faces[2] iUv)
// };

struct Mesh {
    Mesh() = default;
    // FaceIndexType faceIndexType = FaceIndexType::MULTI;
    // int8_t multIndex = -1; // -1 sem multi index
    // std::vector<TrisIndex> faces;
    bool serialized = false;
    std::vector<glm::vec3> point;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec2> uv;
    // std::vector<glm::ivec3> iFacePoint; // iPoint
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

MeshType getMeshTypeFromString(const std::string& text);
void meshDataClean(Mesh* m);
void meshMinMaxSize(Mesh* m, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void meshDataScale(Mesh* m, const float& new_size, const bool& hasTexture);
void meshToTriangle(Mesh* m, std::list<Triangle*>& vTris);
void meshReCompile(Mesh& inData, Mesh& outData);
void meshDebug(Mesh* m, bool _showAll);

} // namespace Chimera