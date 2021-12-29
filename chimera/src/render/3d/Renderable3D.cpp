#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/OpenGLDefs.hpp"

namespace Chimera {

Renderable3D::Renderable3D() : vao(nullptr), ibo(nullptr) {}

Renderable3D::~Renderable3D() {
    if (vao) {
        delete vao;
        vao = nullptr;
    }

    vIndex.clear();
    if (ibo) {
        delete ibo;
        ibo = nullptr;
    }
}

void Renderable3D::addTris(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc) {
    vIndex.push_back(_pa);
    vIndex.push_back(_pb);
    vIndex.push_back(_pc);
}

void Renderable3D::initializeBuffer(VertexData* vertexData, const uint32_t& vertexSize) {
    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(vertexData, vertexSize, &vIndex[0], vIndex.size(), min, max, size);
    aabb.setBoundary(min, max);

    ibo = new IndexBuffer(&vIndex[0], vIndex.size());
}

void Renderable3D::createBuffers(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize) {

    vao = new VertexArray();
    vao->bind();

    VertexBuffer* vbo = new VertexBuffer(BufferType::STATIC);
    vbo->bind();

    BufferLayout layout;
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(2, GL_FLOAT, sizeof(float), false);

    vbo->setLayout(layout);
    vbo->setData(vertexData, vertexSize);
    vbo->unbind();
    vao->push(vbo);

    vao->unbind();

    for (uint32_t i = 0; i < indexSize; i++)
        vIndex.push_back(indexData[i]);

    this->initializeBuffer(vertexData, vertexSize);
}

void Renderable3D::submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) { renderer->submit(command); }

void Renderable3D::debugDados() const {
    glm::vec3 size = aabb.getSize();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "IBO: %d Faces: %ld AABB[%.2f, %.2f, %.2f]", ibo->getBufferID(), vIndex.size() / 3, size.x,
                 size.y, size.z);

    aabb.render();
}

void Renderable3D::draw(const bool& logData) {
    if (ibo != nullptr) { // Desenhar o IBO
        ibo->bind();
        glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        if (logData == true)
            debugDados();

        ibo->unbind();
    }
}

} // namespace Chimera