#include "chimera_render/3d/RenderableArray.hpp"
#include "chimera_core/gl/RenderCommand.hpp"
#include "chimera_render/3d/IRenderer3d.hpp"
#include "chimera_render/3d/RenderableIBO.hpp"
#include <SDL3/SDL.h>

namespace ce {

    RenderableArray::RenderableArray(std::vector<TrisIndex>& v_ptr_tris_index, Mesh* mesh)
        : Renderable3D(), tot_index_(0) {
        // create vertex buffers
        vao = std::make_shared<VertexArray>();
        vao->bind();

        std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>(BufferType::STATIC);
        vbo->bind();

        BufferLayout layout;
        layout.push<float>(3, false);
        layout.push<float>(3, false);
        layout.push<float>(2, false);

        vbo->set_layout(layout);
        vbo->set_data(&mesh->vertex[0], mesh->vertex.size());
        vbo->unbind();

        vao->push(vbo);

        for (auto ptr_tris_index : v_ptr_tris_index) {

            auto [min, max, size] = vertex_indexed_boundaries(mesh->vertex, ptr_tris_index);

            std::shared_ptr<IndexBuffer> ibo =
                std::make_shared<IndexBuffer>((uint32_t*)&ptr_tris_index[0], ptr_tris_index.size() * 3);

            Renderable3D* r = new RenderableIBO(vao, ibo, AABB(min, max));

            v_child_.push_back(r);

            tot_index_ += ptr_tris_index.size();
        }

        vao->unbind();

        auto [min, max, size] = vertex_boundaries(mesh->vertex);

        aabb_.set_boundary(min, max);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Childs: %ld", v_child_.size());
    }

    RenderableArray::~RenderableArray() {

        vao.reset();

        while (!v_child_.empty()) {
            Renderable3D* child = v_child_.back();
            v_child_.pop_back();
            delete child;
            child = nullptr;
        }
    }

    void RenderableArray::submit(RenderCommand& command, IRenderer3d& renderer) {
        for (uint32_t c = 0; c < v_child_.size(); c++)
            renderer.submit(command, v_child_[c], c);
    }
} // namespace ce
