#ifndef VBOINDEXER_HPP
#define VBOINDEXER_HPP

#include <vector>
#include <glm/glm.hpp>
#include "Color.h"

struct VertexData {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
};

void conversorVBO(
    std::vector<unsigned int> & vertexIndex,
    std::vector<glm::vec3> & vertexList,
    std::vector<unsigned int> & normalIndex,
    std::vector<glm::vec3> & normalList,
    std::vector<unsigned int> & textureIndex,
    std::vector<glm::vec2> & textureList,
    std::vector<VertexData> &outData
);

void indexVBO_slow(
    std::vector<VertexData> &inData,
    std::vector<VertexData> &outData,
    std::vector<unsigned int> & out_indices
);

#endif