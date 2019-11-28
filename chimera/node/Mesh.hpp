#ifndef __CHIMERA_MESH__HPP
#define __CHIMERA_MESH__HPP

#include <string.h>
#include <tinyxml2.h>
#include <vector>

#include "Node.hpp"
#include "chimera/core/Material.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/Vertex.hpp"
#include "chimera/render/MeshData.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class Mesh : public Node {
  public:
    Mesh(Node* _parent, std::string _name);
    Mesh(const Mesh& _cpy);

    virtual ~Mesh();

    virtual void init() override;
    virtual void accept(class NodeVisitor* v) override;

    void render(Shader* _pShader);

    void replaceTransform(Transform* _pTransform);

    void debugDados();

    MeshData meshData;

    inline void setMaterial(Material* _pMat) { this->material = _pMat; }
    inline Material* getMaterial() const { return material; }

    inline Transform* getTransform() { return pTransform; }
    inline void setTransform(Transform* _pTransform) { pTransform = _pTransform; }

  private:
    void setVertexBuffer();
    VertexRenderStatic renderStat;
    Material* material;
    Transform* pTransform;
};

Mesh* createEmpty(Node* _pParent, const std::string& _name, Material* _pMaterial);

} // namespace Chimera
#endif
