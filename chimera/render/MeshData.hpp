#ifndef __CHIMERA_MESH_DATA_HPP
#define __CHIMERA_MESH_DATA_HPP

#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

struct MeshData {
    std::vector<glm::vec3> vertexList;
    std::vector<glm::vec3> normalList;
    std::vector<glm::vec2> textureList;
    std::vector<unsigned int> vertexIndex;
    std::vector<unsigned int> normalIndex;
    std::vector<unsigned int> textureIndex;
};

void meshDataDestroy(MeshData& m);
void meshDataMinMaxSize(const MeshData& m, glm::vec3& min, glm::vec3& max, glm::vec3& size);
void meshDataChangeSize(MeshData& m, const float& new_size, const bool& hasTexture);
void meshDataDebugDados(const MeshData& m);

} // namespace Chimera
#endif