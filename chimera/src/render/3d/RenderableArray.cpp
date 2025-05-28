#include "chimera/render/3d/RenderableArray.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderableIBO.hpp"
#include <SDL3/SDL.h>

namespace ce {

    RenderableArray::RenderableArray(std::vector<TrisIndex>& vPtrTrisIndex, Mesh* mesh) : Renderable3D(), totIndex(0) {
        // create vertex buffers
        vao = std::make_shared<VertexArray>();
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

            std::shared_ptr<IndexBuffer> ibo =
                std::make_shared<IndexBuffer>((uint32_t*)&ptrTrisIndex[0], ptrTrisIndex.size() * 3);

            Renderable3D* r = new RenderableIBO(vao, ibo, AABB(min, max));

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
            Renderable3D* child = vChild.back();
            vChild.pop_back();
            delete child;
            child = nullptr;
        }
    }

    void RenderableArray::submit(RenderCommand& command, IRenderer3d& renderer) {
        for (uint32_t c = 0; c < vChild.size(); c++)
            renderer.submit(command, vChild[c], c);
    }
} // namespace ce
