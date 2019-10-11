#ifndef __CHIMERA_LOAD_OBJ__HPP
#define __CHIMERA_LOAD_OBJ__HPP

#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

struct MeshData {
    std::vector<unsigned int> vertexIndex;
    std::vector<glm::vec3> vertexList;
    std::vector<glm::vec3> colorList;

    std::vector<unsigned int> normalIndex;
    std::vector<glm::vec3> normalList;

    std::vector<unsigned int> textureIndex;
    std::vector<glm::vec2> textureList;
};

void loadObj(const char* _fineName, MeshData* _mesh);

} // namespace Chimera

#endif