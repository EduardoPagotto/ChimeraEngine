#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/OpenGLDefs.hpp"

namespace Chimera {

Renderable3D::Renderable3D() : vao(nullptr), entity(Entity()) {}

Renderable3D::~Renderable3D() {
    if (vao) {
        delete vao;
        vao = nullptr;
    }
}

void Renderable3D::addTris(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc) { poligonIndex.addTris(_pa, _pb, _pc); }

void Renderable3D::initializeBuffer(VertexData* vertexData, const uint32_t& vertexSize) {
    poligonIndex.initializeBuffer(vertexData, vertexSize);
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

    poligonIndex.addTrisEntire(indexData, indexSize);
    poligonIndex.initializeBuffer(vertexData, vertexSize);
}

void Renderable3D::setEntity(Entity entity) { this->entity = entity; }

void Renderable3D::submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) { renderer->submit(command); }

void Renderable3D::debugDados() { poligonIndex.debugDados(); }
} // namespace Chimera