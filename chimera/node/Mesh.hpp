#ifndef __CHIMERA_MESH__HPP
#define __CHIMERA_MESH__HPP

#include <string.h>
#include <tinyxml2.h>
#include <vector>

#include "Coord.hpp"
#include "Node.hpp"
#include "chimera/render/MatData.hpp"
#include "chimera/render/MeshData.hpp"
#include "chimera/render/Vertex.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class Mesh : public Node {
  public:
    Mesh(Node* _parent, std::string _name);
    Mesh(const Mesh& _cpy);

    virtual ~Mesh();
    virtual void init();

    // Inherited via Node
    void accept(class NodeVisitor* v) override;

    void render(Shader* _pShader);
    glm::vec3 getSizeBox();

    void replaceCoord(Coord* _pCoord);

    void debugDados();

    MeshData meshData;

    void setMaterial(MatData* _pMat) { this->material = _pMat; }
    MatData* getMaterial() const { return material; }

    inline Coord* getCoord() { return pCoord; }
    void setCoord(Coord* _pCoord) { pCoord = _pCoord; }

  private:
    void setVertexBuffer();
    VertexRenderStatic renderStat;
    MatData* material;
    Coord* pCoord;
};

Mesh* createEmpty(Node* _pParent, const std::string& _name, MatData* _pMaterial);

} // namespace Chimera
#endif
