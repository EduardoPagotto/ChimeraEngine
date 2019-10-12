#ifndef __CHIMERA_VBOINDEXER_HPP
#define __CHIMERA_VBOINDEXER_HPP

#include "Vertex.hpp"
#include <vector>

namespace Chimera {

void conversorVBO(std::vector<unsigned int>& vertexIndex, std::vector<glm::vec3>& vertexList,
                  std::vector<unsigned int>& normalIndex, std::vector<glm::vec3>& normalList,
                  std::vector<unsigned int>& textureIndex, std::vector<glm::vec2>& textureList,
                  // Chimera::Color cor,
                  std::vector<VertexData>& outData);

// void indexVBO_slow(std::vector<VertexData>& inData, std::vector<VertexData>& outData,
//                    std::vector<unsigned int>& out_indices);

} // namespace Chimera
#endif