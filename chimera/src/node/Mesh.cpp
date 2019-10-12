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
    // // conversorVBO(vertexIndex, vertexList, normalIndex, normalList, textureIndex, textureList, vertexDataIn);
    // indexVBO_slow(vertexDataIn, vertexData, indexIBO);

    // SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "VBO Nome: %s Indice: %d Data: %d", getName().c_str(),
    // (int)indexIBO.size(),
    //              (int)vertexData.size());

    // unsigned int sizeBufferVertex = vertexData.size() * sizeof(VertexData);
    // unsigned int sizeBufferIndex = indexIBO.size() * sizeof(unsigned int);

    // // Buffer de vertice
    // glGenBuffers(1, &VertexVBOID);
    // glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
    // glBufferData(GL_ARRAY_BUFFER, sizeBufferVertex, &vertexData[0], GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // // Buffer de indice
    // glGenBuffers(1, &IndexVBOID);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeBufferIndex, &indexIBO[0], GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // // cria o VAO
    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);

    // // vincula VBO
    // glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);

    // // Vertice
    // GLuint positionID = 0; // glGetAttribLocation(shader.getIdProgram(), "position");
    // glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0));
    // glEnableVertexAttribArray(positionID);

    // // Normal
    // GLuint normalID = 1; // glGetAttribLocation(shader.getIdProgram(), "normal");
    // glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(12));
    // glEnableVertexAttribArray(normalID);

    // // Texture
    // GLuint uvID = 2; // glGetAttribLocation(shader.getIdProgram(), "uv1");
    // glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(24));
    // glEnableVertexAttribArray(uvID);

    // // vincula IBO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

    // // limpa dados
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
    // glDisableVertexAttribArray(positionID);
    // glDisableVertexAttribArray(normalID);
    // glDisableVertexAttribArray(uvID);
}
} // namespace Chimera
