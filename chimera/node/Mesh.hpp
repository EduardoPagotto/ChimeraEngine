#ifndef __CHIMERA_MESH__HPP
#define __CHIMERA_MESH__HPP

#include <string.h>
#include <tinyxml2.h>
#include <vector>

#include "Draw.hpp"
#include "chimera/render/MeshData.hpp"
#include "chimera/render/Vertex.hpp"
#include <glm/glm.hpp>

namespace Chimera {

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

    void debugDados();

    MeshData meshData;

  private:
    void setVertexBuffer();
    VertexRenderStatic renderStat;
};

Mesh* createEmpty(Node* _pParent, const std::string& _name, MatData* _pMaterial);

} // namespace Chimera
#endif
