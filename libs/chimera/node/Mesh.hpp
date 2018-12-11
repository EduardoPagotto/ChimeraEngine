#ifndef __CHIMERA_MESH__HPP
#define __CHIMERA_MESH__HPP

#include <string.h>
#include <tinyxml2.h>
#include <vector>

#include "Draw.hpp"
#include "chimera/core/VboIndexer.hpp"
#include <glm/glm.hpp>

#include <spdlog/spdlog.h>

namespace Chimera {

// Define this somewhere in your header file
#define BUFFER_OFFSET(i) ((void*)(i))

class Mesh : public Draw {
  public:
    Mesh(Node* _parent, std::string _name);
    Mesh(const Mesh& _cpy);

    virtual ~Mesh();
    virtual void init();

    // Inherited via Node
    void accept(class NodeVisitor* v) override;

    // Inherited via Draw
    virtual void render(Shader* _pShader) override;
    virtual glm::vec3 getSizeBox();

    std::vector<unsigned int> vertexIndex;
    std::vector<glm::vec3> vertexList;

    std::vector<unsigned int> normalIndex;
    std::vector<glm::vec3> normalList;

    std::vector<unsigned int> textureIndex;
    std::vector<glm::vec2> textureList;

  private:
    void setVertexBuffer();

    GLuint VAO;
    GLuint VertexVBOID;
    GLuint IndexVBOID;

    std::vector<unsigned int> indexIBO;
    std::vector<VertexData> vertexData;

    std::shared_ptr<spdlog::logger> log;
};
} // namespace Chimera
#endif
