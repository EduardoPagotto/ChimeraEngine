#ifndef __CHIMERA_VERTEX_HPP
#define __CHIMERA_VERTEX_HPP

#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

struct VertexData {
    glm::vec3 position; // 3 * 4 = 12 (0 -11)
    glm::vec3 normal;   // 3 * 4 = 12 (12-23)
    glm::vec2 texture;  // 2 * 4 = 08 (24-31)
    // glm::vec4 color;    //4 * 4 = 16 (32-47) //cuidado com a posicao, muda o
    // sizeof(VertexData)
};

struct VertexDataFull {
    glm::vec3 position; // 3 * 4 = 12 ( 0 - 11)
    glm::vec3 normal;   // 3 * 4 = 12 (12 - 23)
    glm::vec3 color;    // 3 * 4 = 12 (24 - 35)
    glm::vec2 texture;  // 2 * 4 = 08 (36 - 43) //cuidado, posicao muda o sizeof(VertexDataFull)
};

class VertexFull {
  public:
    VertexFull();
    void create(const unsigned int& max);
    virtual ~VertexFull();
    void render(std::vector<VertexDataFull>& vVertice);

  private:
    unsigned int VBO, VAO;
};

// void conversorVBO(std::vector<unsigned int>& vertexIndex, std::vector<glm::vec3>& vertexList,
//                   std::vector<unsigned int>& normalIndex, std::vector<glm::vec3>& normalList,
//                   std::vector<unsigned int>& textureIndex, std::vector<glm::vec2>& textureList,
//                   // Chimera::Color cor,
//                   std::vector<VertexData>& outData);

// void indexVBO_slow(std::vector<VertexData>& inData, std::vector<VertexData>& outData,
//                    std::vector<unsigned int>& out_indices);
} // namespace Chimera
#endif