#include "chimera/render/3d/RenderableArray.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/3d/RenderableIBO.hpp"
#include "chimera/render/VertexData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

RenderableArray::RenderableArray(std::vector<TrisIndex>& vPtrTrisIndex, Mesh* mesh) : totIndex(0) {
    // create vertex buffers
    vao = new VertexArray();
    vao->bind();

    VertexBuffer* vbo = new VertexBuffer(BufferType::STATIC);
    vbo->bind();

    BufferLayout layout;
    layout.Push<float>(3, false);
    layout.Push<float>(3, false);
    layout.Push<float>(2, false);

    std::vector<VertexData> vertexData;
    vertexDataFromMesh(mesh, vertexData);

    vbo->setLayout(layout);
    vbo->setData(&vertexData[0], vertexData.size());
    vbo->unbind();

    vao->push(vbo);

    for (auto ptrTrisIndex : vPtrTrisIndex) {

        glm::vec3 min, max, size;
        vertexDataIndexMinMaxSize(&vertexData[0], vertexData.size(), &ptrTrisIndex.vIndex[0], ptrTrisIndex.size(), min, max, size);

        IndexBuffer* ibo = new IndexBuffer(&ptrTrisIndex.vIndex[0], ptrTrisIndex.size());
        IRenderable3d* r = new RenderableIBO(ibo, AABB(min, max));

        vChild.push_back(r);

        totIndex += ptrTrisIndex.size();
    }

    vao->unbind();

    glm::vec3 min, max, size;
    vertexDataMinMaxSize(&vertexData[0], vertexData.size(), min, max, size);
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

    delete vao;
    vao = nullptr;
}

void RenderableArray::draw(const bool& logData) {
    if (logData)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "RenderableArray draw"); // TODO: ver o que fazer
}

void RenderableArray::submit(RenderCommand& command, IRenderer3d& renderer) {

    if (renderer.submit(command, this) == true) {
        for (IRenderable3d* child : vChild)
            renderer.submit(command, child);
    }
}
} // namespace Chimera