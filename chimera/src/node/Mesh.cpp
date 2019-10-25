#include <iterator>

#include "chimera/core/Singleton.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/node/NodeVisitor.hpp"

#include "chimera/render/LoadObj.hpp"

#include <SDL2/SDL.h>

namespace Chimera {

Mesh::Mesh(Node* _parent, std::string _name) : Node(_parent, Kind::MESH, _name) {
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Constructor Mesh: %s", _name.c_str());
}

Mesh::Mesh(const Mesh& _cpy) : Node(_cpy) {

    // FIXME: criar a copia do meshdata

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Constructor Copy Mesh %s", _cpy.getName().c_str());
}

Mesh::~Mesh() {

    // FIXME: remover o mesh data

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Destructor Mesh: %s", getName().c_str());
}

void Mesh::init() {

    if (material == nullptr)
        material = new MatData();

    material->init();

    meshData.textureFix();

    pTransform->init(meshData.getSizeBox());

    setVertexBuffer();
}

void Mesh::accept(NodeVisitor* v) { v->visit(this); }

glm::vec3 Mesh::getSizeBox() { return meshData.getSizeBox(); }

void Mesh::render(Shader* _pShader) { renderStat.render(); }

void Mesh::setVertexBuffer() {
    std::vector<VertexData> vertexDataIn;
    convertMeshDataVertexData(&meshData, vertexDataIn);

    renderStat.create(vertexDataIn);
}

void Mesh::debugDados() {
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Debug Mess Nome: %s", getName().c_str());
    meshData.debugDados();
}

void Mesh::replaceTransform(Transform* _pTransform) {

    glm::mat4 matrix = pTransform->getMatrix();
    _pTransform->setMatrix(matrix);

    delete pTransform;
    pTransform = nullptr;

    pTransform = _pTransform;
}

Mesh* createEmpty(Node* _pParent, const std::string& _name, MatData* _pMaterial) {

    // Mesh
    Mesh* pMesh = new Mesh(_pParent, _name);
    MatData* pMatFinal = _pMaterial;
    if (pMatFinal == nullptr)
        pMatFinal = new MatData();

    pMesh->setMaterial(_pMaterial);
    return pMesh;
}

} // namespace Chimera
