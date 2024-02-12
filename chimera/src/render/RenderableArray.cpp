#include "chimera/render/3d/RenderableArray.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderableIBO.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableArray::RenderableArray(std::vector<TrisIndex>& vPtrTrisIndex, Mesh* mesh) : totIndex(0), Renderable3D() {
    // create vertex buffers
    vao = new VertexArray();
    vao->bind();

    VertexBuffer* vbo = new VertexBuffer(BufferType::STATIC);
    vbo->bind();

    BufferLayout layout;
    layout.Push<float>(3, false);
    layout.Push<float>(3, false);
    layout.Push<float>(2, false);

    vbo->setLayout(layout);
    vbo->setData(&mesh->vertex[0], mesh->vertex.size());
    vbo->unbind();

    vao->push(vbo);

    for (auto ptrTrisIndex : vPtrTrisIndex) {

        auto [min, max, size] = vertexIndexedBoundaries(mesh->vertex, ptrTrisIndex);

        IndexBuffer* ibo = new IndexBuffer((uint32_t*)&ptrTrisIndex[0], ptrTrisIndex.size() * 3);
        IRenderable3d* r = new RenderableIBO(vao, ibo, AABB(min, max));

        vChild.push_back(r);

        totIndex += ptrTrisIndex.size();
    }

    vao->unbind();

    auto [min, max, size] = vertexBoundaries(mesh->vertex);

    aabb.setBoundary(min, max);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Childs: %ld", vChild.size());
}

RenderableArray::~RenderableArray() {
    while (!vChild.empty()) {
        IRenderable3d* child = vChild.back();
        vChild.pop_back();
        delete child;
        child = nullptr;
    }
}

void RenderableArray::submit(RenderCommand& command, IRenderer3d& renderer) {
    for (uint32_t c = 0; c < vChild.size(); c++)
        renderer.submit(command, vChild[c], c);
}
} // namespace Chimera