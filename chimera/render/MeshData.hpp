#ifndef __CHIMERA_MESH_DATA_HPP
#define __CHIMERA_MESH_DATA_HPP

#include "chimera/core/VertexData.hpp"
#include "chimera/render/Triangle.hpp"
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
    void toTriangle(std::vector<Triangle>& vecTriangle);

    inline void addVertice(const glm::vec3& _vertice) { vertexList.push_back(_vertice); }
    inline void addNormal(const glm::vec3& _normal) { normalList.push_back(_normal); }
    inline void addUV(const glm::vec2& _uv) { textureList.push_back(_uv); }

    inline void addVerticeIndex(const unsigned int& _indice) { vertexIndex.push_back(_indice); }
    inline void addNormalIndex(const unsigned int& _indice) { normalIndex.push_back(_indice); }
    inline void addUVIndex(const unsigned int& _indice) { textureIndex.push_back(_indice); }

    inline void setOneIndex(const bool& _oneIndex) { oneIndex = _oneIndex; }
    inline bool getOneIndex() const { return oneIndex; }
    inline std::vector<unsigned int> getVertexIndex() { return vertexIndex; }
    inline std::vector<glm::vec3> getVertexList() { return vertexList; }

  private:
    void _toVertexDataNotOneIndex(std::vector<VertexData>& outData);
    void _toVertexDataOneIndex(std::vector<VertexData>& outData);
    void _toTriangleNotOneIndex(std::vector<Triangle>& vecTriangle);
    void _toTriangleOneIndex(std::vector<Triangle>& vecTriangle);

    bool oneIndex;
    std::vector<glm::vec3> vertexList;
    std::vector<glm::vec3> normalList;
    std::vector<glm::vec2> textureList;
    std::vector<unsigned int> vertexIndex;
    std::vector<unsigned int> normalIndex;
    std::vector<unsigned int> textureIndex;
};
} // namespace Chimera
#endif