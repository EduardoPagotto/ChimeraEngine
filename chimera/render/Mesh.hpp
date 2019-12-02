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

    MeshData* getMeshData() { return pData; }

  private:
    AABB aabb;
    MeshData* pData;
    Material* pMat;
    Transform* pTrans;
};
} // namespace Chimera

#endif