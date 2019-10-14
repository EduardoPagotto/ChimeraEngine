#ifndef __CHIMERA_VERTEX_HPP
#define __CHIMERA_VERTEX_HPP

#include <glm/glm.hpp>
#include <vector>

// Tutorial opengl shadeGL - https://learnopengl.com/Getting-started/Shaders

namespace Chimera {

struct VertexData {
    glm::vec3 position; // 3 * 4 = 12 ( 0 - 11)
    glm::vec3 normal;   // 3 * 4 = 12 (12 - 23)
    glm::vec2 texture;  // 2 * 4 = 08 (24 - 31)
};

void indexVBO_slow(std::vector<VertexData>& inData, std::vector<VertexData>& outData,
                   std::vector<unsigned int>& out_indices);

class VertexRenderDynamic {
  public:
    VertexRenderDynamic();
    void create(const unsigned int& max);
    virtual ~VertexRenderDynamic();
    void render(std::vector<VertexData>& vVertice);

  private:
    unsigned int VBO, VAO;
};

class VertexRenderStatic {
  public:
    VertexRenderStatic();
    virtual ~VertexRenderStatic();
    void create(std::vector<VertexData>& vertexDataIn); // void setVertexBuffer();
    void render();

  private:
    unsigned int VAO;
    unsigned int VertexVBOID;
    unsigned int IndexVBOID;
    std::vector<unsigned int> indexIBO;
    std::vector<VertexData> vertexData;
};

} // namespace Chimera
#endif