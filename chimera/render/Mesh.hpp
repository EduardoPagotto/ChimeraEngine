#ifndef __CHIMERA_MESH__HPP
#define __CHIMERA_MESH__HPP

#include "chimera/core/Transform.hpp"
#include "chimera/render/AABB.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/MeshData.hpp"

namespace Chimera {
class Mesh {
  public:
    Mesh();
    virtual ~Mesh();

    void init(MeshData* _pData3D, Material* _pMaterial);
    void destroy();

    inline MeshData* getMeshData() { return pData3D; }
    inline AABB* getAABB() { return pAABB; }
    inline Transform* getTransform() { return pTrans; }

  private:
    AABB* pAABB;
    MeshData* pData3D;
    Transform* pTrans;
    Material* pMat;
};
} // namespace Chimera

#endif