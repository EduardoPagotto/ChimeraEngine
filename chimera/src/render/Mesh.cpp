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

void Mesh::init(Transform* _pTrans, MeshData* _pData3D, Material* _pMaterial) {

    if (_pMaterial != nullptr) {
        this->pMat = _pMaterial;
    } else {
        this->pMat = new Material();
        this->pMat->setDefaultEffect();
    }

    glm::vec3 min, max, size;

    this->pData3D = _pData3D;
    this->pData3D->getMinMaxSize(min, max, size);

    pAABB = new AABB(min, max);

    pTrans = _pTrans;
    if (pTrans != nullptr)
        pTrans->init(size);
}

} // namespace Chimera