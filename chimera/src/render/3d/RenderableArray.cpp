#include "chimera/render/3d/RenderableArray.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableArray::RenderableArray(VecPrtTrisIndex& vPtrTrisIndex, std::vector<VertexData>& vertexData) : totIndex(0) {

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

    for (auto ptrTrisIndex : vPtrTrisIndex) {

        glm::vec3 min, max, size;
        vertexDataIndexMinMaxSize(&this->vVertex[0], this->vVertex.size(), &ptrTrisIndex->vIndex[0], ptrTrisIndex->size(), min, max, size);

        IndexBuffer* ibo = new IndexBuffer(&ptrTrisIndex->vIndex[0], ptrTrisIndex->size());
        Renderable3D* r = new Renderable3D(nullptr, ibo, ptrTrisIndex->size(), AABB(min, max));

        vChild.push_back(r);

        totIndex += ptrTrisIndex->size();
    }

    vao->unbind();

    glm::vec3 min, max, size;
    vertexDataMinMaxSize(&vVertex[0], vVertex.size(), min, max, size);
    aabb.setBoundary(min, max);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Childs: %ld", vChild.size());
}

RenderableArray::~RenderableArray() {

    while (!vChild.empty()) {
        Renderable3D* child = vChild.back();
        vChild.pop_back();
        delete child;
        child = nullptr;
    }

    delete vao;
    vao = nullptr;
}

void RenderableArray::draw(const bool& logData) {
    if (logData)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "RenderableArray draw"); // TODO: ver o que fazer
}

void RenderableArray::submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) {

    renderer->submit(command);

    for (Renderable3D* child : vChild) {
        command.renderable = child;
        renderer->submit(command);
    }
}
} // namespace Chimera