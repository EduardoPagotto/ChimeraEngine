#include "chimera/node/NodeMesh.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/node/VisitorInterface.hpp"
#include "chimera/render/LoadObj.hpp"
#include <SDL2/SDL.h>
#include <iterator>

namespace Chimera {

NodeMesh::NodeMesh(Node* _parent, std::string _name) : Node(_parent, Kind::MESH, _name) {
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Constructor NodeMesh: %s", _name.c_str());
}

NodeMesh::NodeMesh(const NodeMesh& _cpy) : Node(_cpy) {
    // FIXME: criar a copia do meshdata
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Constructor Copy NodeMesh %s", _cpy.getName().c_str());
}

NodeMesh::~NodeMesh() {
    // FIXME: remover o NodeMesh data
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Destructor NodeMesh: %s", getName().c_str());
}

void NodeMesh::init() {

    if (material == nullptr)
        material = new Material();

    material->init();

    glm::vec3 min, max, size;
    meshData.getMinMaxSize(min, max, size);

    aabb.setBoundary(min, max);

    pTransform->init(size);

    setVertexBuffer();
}

void NodeMesh::accept(VisitorInterface* v) { v->visit(this); }

void NodeMesh::render(Shader* _pShader) { renderStat.render(); }

void NodeMesh::setVertexBuffer() {
    std::vector<VertexData> vertexDataIn;
    meshData.toVertexData(vertexDataIn);

    if (meshData.getOneIndex() == false) {
        std::vector<unsigned int> index;
        renderStat.create(vertexDataIn, index);
    } else {
        renderStat.create(vertexDataIn, meshData.getVertexIndex());
    }
}

void NodeMesh::debugDados(bool _showAll) {
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Debug Mess Nome: %s", getName().c_str());
    meshData.debugDados(_showAll);
}

void NodeMesh::replaceTransform(Transform* _pTransform) {

    glm::mat4 matrix = pTransform->getMatrix();
    _pTransform->setMatrix(matrix);

    delete pTransform;
    pTransform = nullptr;

    pTransform = _pTransform;
}

NodeMesh* createEmpty(Node* _pParent, const std::string& _name, Material* _pMaterial) {

    // NodeMesh
    NodeMesh* pMesh = new NodeMesh(_pParent, _name);
    Material* pMatFinal = _pMaterial;
    if (pMatFinal == nullptr)
        pMatFinal = new Material();

    pMesh->setMaterial(_pMaterial);
    return pMesh;
}

} // namespace Chimera
