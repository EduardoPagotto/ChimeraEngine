#ifndef __CHIMERA_MESH_DATA_HPP
#define __CHIMERA_MESH_DATA_HPP

#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

class MeshData {

  public:
    MeshData();
    MeshData(const MeshData& _cpy);
    virtual ~MeshData();

    void textureFix();
    glm::vec3 getSizeBox();

    void changeSize(const float& new_size, bool hasTexture);

    void debugDados();

    std::vector<unsigned int> vertexIndex;
    std::vector<glm::vec3> vertexList;
    std::vector<glm::vec3> colorList;

    std::vector<unsigned int> normalIndex;
    std::vector<glm::vec3> normalList;

    std::vector<unsigned int> textureIndex;
    std::vector<glm::vec2> textureList;
};

} // namespace Chimera
#endif