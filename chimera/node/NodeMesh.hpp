#ifndef __CHIMERA_NODE_MESH__HPP
#define __CHIMERA_NODE_MESH__HPP

#include "Node.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/VertexData.hpp"
#include "chimera/render/AABB.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/vbs/RenderableStatic.hpp"
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
    virtual void accept(class VisitorInterface* v) override;

    void replaceTransform(Transform* _pTransform);

    void debugDados(bool _showAll);

    MeshData meshData;

    inline void setMaterial(Material* _pMat) { this->material = _pMat; }
    inline Material* getMaterial() const { return material; }

    inline Transform* getTransform() { return pTransform; }
    inline void setTransform(Transform* _pTransform) { pTransform = _pTransform; }

    // FIXME: NodeMesh virar classe de renderizacao!!!!!
    RenderableStatic* pRenderStat;

  private:
    void setVertexBuffer();

    AABB aabb;
    Material* material;
    Transform* pTransform;
};

NodeMesh* createEmpty(Node* _pParent, const std::string& _name, Material* _pMaterial);

} // namespace Chimera
#endif
