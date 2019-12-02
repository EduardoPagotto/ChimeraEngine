#ifndef __CHIMERA_MESH__HPP
#define __CHIMERA_MESH__HPP

#include "Node.hpp"
#include "chimera/core/Material.hpp"
#include "chimera/core/MeshData.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/VertexStatic.hpp"
#include <glm/glm.hpp>
#include <string.h>
#include <tinyxml2.h>
#include <vector>

namespace Chimera {

class NodeMesh : public Node {
  public:
    NodeMesh(Node* _parent, std::string _name);
    NodeMesh(const NodeMesh& _cpy);

    virtual ~NodeMesh();

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

NodeMesh* createEmpty(Node* _pParent, const std::string& _name, Material* _pMaterial);

} // namespace Chimera
#endif
