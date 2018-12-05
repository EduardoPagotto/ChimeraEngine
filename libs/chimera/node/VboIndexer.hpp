#ifndef __CHIMERA_VBOINDEXER_HPP
#define __CHIMERA_VBOINDEXER_HPP

#include "Color.hpp"
#include <glm/glm.hpp>
#include <vector>

struct VertexData {
    glm::vec3 position; // 3 * 4 = 12 (0 -11)
    glm::vec3 normal;   // 3 * 4 = 12 (12-23)
    glm::vec2 texture;  // 2 * 4 = 08 (24-31)
    // glm::vec4 color;    //4 * 4 = 16 (32-47) //cuidado com a posicao, muda o
    // sizeof(VertexData)
};

void conversorVBO(std::vector<unsigned int>& vertexIndex, std::vector<glm::vec3>& vertexList,
                  std::vector<unsigned int>& normalIndex, std::vector<glm::vec3>& normalList,
                  std::vector<unsigned int>& textureIndex, std::vector<glm::vec2>& textureList,
                  // Chimera::Color cor,
                  std::vector<VertexData>& outData);

void indexVBO_slow(std::vector<VertexData>& inData, std::vector<VertexData>& outData,
                   std::vector<unsigned int>& out_indices);

#endif