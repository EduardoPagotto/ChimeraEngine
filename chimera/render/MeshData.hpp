#ifndef __CHIMERA_MESH_DATA_HPP
#define __CHIMERA_MESH_DATA_HPP

#include "chimera/render/Triangle.hpp"
#include "chimera/render/VertexData.hpp"
#include <vector>
namespace Chimera {
class MeshData {
  public:
    MeshData();
    ~MeshData();
    void destroy();
    void getMinMaxSize(glm::vec3& min, glm::vec3& max, glm::vec3& size);
    void changeSize(const float& new_size, const bool& hasTexture);
    void debugDados(bool _showAll);

    void toVertexData(std::vector<VertexData>& outData);
    void toTriangle(std::vector<Triangle>& vecTriangle, std::vector<unsigned int>& _index);

    inline void addVertex(const glm::vec3& _vertex) { vertexList.push_back(_vertex); }
    inline void addNormal(const glm::vec3& _normal) { normalList.push_back(_normal); }
    inline void addUV(const glm::vec2& _uv) { uvList.push_back(_uv); }

    inline void addVertexIndex(const unsigned int& _indice) { vertexIndex.push_back(_indice); }
    inline void addNormalIndex(const unsigned int& _indice) { normalIndex.push_back(_indice); }
    inline void addUVIndex(const unsigned int& _indice) { uvIndex.push_back(_indice); }

    inline void setSingleIndex(const bool& _oneIndex) { singleIndex = _oneIndex; }
    inline bool isSingleIndex() const { return singleIndex; }
    inline std::vector<unsigned int> getVertexIndex() { return vertexIndex; }
    inline std::vector<glm::vec3> getVertexList() { return vertexList; }

  private:
    bool singleIndex;
    std::vector<glm::vec3> vertexList;
    std::vector<glm::vec3> normalList;
    std::vector<glm::vec2> uvList;
    std::vector<unsigned int> vertexIndex;
    std::vector<unsigned int> normalIndex;
    std::vector<unsigned int> uvIndex;
};
} // namespace Chimera
#endif