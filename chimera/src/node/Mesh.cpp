#include <iterator>

#include "chimera/core/Singleton.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/node/MeshUtil.hpp"
#include "chimera/node/NodeVisitor.hpp"

#include "chimera/core/LoadObj.hpp"

#include <SDL2/SDL.h>

// Tutorial opengl shadeGL - https://learnopengl.com/Getting-started/Shaders

namespace Chimera {

Mesh::Mesh(Node* _parent, std::string _name) : Draw(_parent, EntityKind::MESH, _name) {
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Constructor Mesh: %s", _name.c_str());
}

Mesh::Mesh(const Mesh& _cpy) : Draw(_cpy) {

    // FIXME: criar a copia do meshdata

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Constructor Copy Mesh %s", _cpy.getName().c_str());
}

Mesh::~Mesh() {

    // FIXME: remover o mesh data

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Destructor Mesh: %s", getName().c_str());
}

void Mesh::init() {

    if (material == nullptr)
        material = new Material("DefaultMat-" + std::to_string(getSerial()));

    material->init();

    meshData.textureFix();

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
} // namespace Chimera
