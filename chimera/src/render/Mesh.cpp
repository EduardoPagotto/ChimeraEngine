#include "chimera/render/Mesh.hpp"

namespace Chimera {

Mesh::Mesh() {
    pData3D = nullptr;
    pAABB = nullptr;
    pTrans = nullptr;
    pMat = nullptr;
}

Mesh::~Mesh() { this->destroy(); }

void Mesh::destroy() {

    if (pData3D != nullptr) {
        meshDataDestroy(*pData3D);
        delete pData3D;
        pData3D = nullptr;
    }

    if (pAABB != nullptr) {
        delete pAABB;
        pAABB = nullptr;
    }

    if (pTrans != nullptr) {
        delete pTrans;
        pTrans = nullptr;
    }

    if (pMat != nullptr) {
        delete pMat;
        pMat = nullptr;
    }
}

void Mesh::init(MeshData* _pData3D, Material* _pMaterial) {
    this->pData3D = _pData3D;

    if (_pMaterial != nullptr) {
        this->pMat = _pMaterial;
    } else {
        this->pMat = new Material();
        this->pMat->setDefaultEffect();
    }

    glm::vec3 size, min, max;

    meshDataMinMaxSize(*_pData3D, min, max, size);

    pAABB = new AABB(min, max);
    pTrans = new Transform();
    pTrans->init(size);
}

} // namespace Chimera