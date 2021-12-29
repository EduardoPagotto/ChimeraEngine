#include "chimera/render/3d/RenderableChunk.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableChunk::RenderableChunk(std::vector<Renderable3D*>& vChild, std::vector<VertexData>& vertexData) : totIndex(0) {
    this->vChild = std::move(vChild);
    this->vVertex = std::move(vertexData);

    // create vertex buffers
    vao = new VertexArray();
    vao->bind();

    VertexBuffer* vbo = new VertexBuffer(BufferType::STATIC);
    vbo->bind();

    BufferLayout layout;
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(2, GL_FLOAT, sizeof(float), false);

    vbo->setLayout(layout);
    vbo->setData(&this->vVertex[0], this->vVertex.size());
    vbo->unbind();

    vao->push(vbo);

    for (Renderable3D* child : this->vChild) {
        child->initializeBuffer(&vVertex[0], vVertex.size());
        totIndex += child->getSize();
    }

    vao->unbind();

    glm::vec3 min, max, size;
    vertexDataMinMaxSize(&vVertex[0], vVertex.size(), min, max, size);
    aabb.setBoundary(min, max);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Childs: %ld", vChild.size());
}

RenderableChunk::~RenderableChunk() {

    while (!vChild.empty()) {
        Renderable3D* child = vChild.back();
        vChild.pop_back();
        delete child;
        child = nullptr;
    }

    delete vao;
    vao = nullptr;
}

void RenderableChunk::draw(const bool& logData) {
    if (logData)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "RenderableChunk draw"); // TODO: ver o que fazer
}

void RenderableChunk::submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) {

    renderer->submit(command);

    for (Renderable3D* child : vChild) {
        command.renderable = child;
        renderer->submit(command);
    }
}
} // namespace Chimera