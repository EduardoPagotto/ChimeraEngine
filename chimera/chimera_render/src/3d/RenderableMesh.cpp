#include "chimera_render/3d/RenderableMesh.hpp"
#include "chimera_core/gl/RenderCommand.hpp"
#include "chimera_render/3d/IRenderer3d.hpp"
#include "chimera_render/3d/RenderableIBO.hpp"

namespace ce {

    RenderableMesh::RenderableMesh(Mesh* mesh) : Renderable3D() {

        Mesh temp;
        meshReindex(*mesh, temp);

        // Create VAO, VBO and IBO
        vao = std::make_shared<VertexArray>();
        vao->bind();

        std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>(BufferType::STATIC);
        vbo->bind();

        BufferLayout layout;
        layout.Push<float>(3, false);
        layout.Push<float>(3, false);
        layout.Push<float>(2, false);

        vbo->setLayout(layout);
        vbo->setData(&temp.vertex[0], temp.vertex.size());
        vbo->unbind();
        vao->push(vbo);
        vao->unbind();

        auto [min, max, size] = vertexIndexedBoundaries(temp.vertex, temp.iFace);

        aabb_.setBoundary(min, max);

        std::shared_ptr<IndexBuffer> ibo =
            std::make_shared<IndexBuffer>((uint32_t*)&temp.iFace[0], temp.iFace.size() * 3);

        tot_index_ = ibo->get_size();

        child_ = new RenderableIBO(vao, ibo, AABB(min, max));
    }

    RenderableMesh::~RenderableMesh() {
        delete child_;
        child_ = nullptr;
    }

    void RenderableMesh::submit(RenderCommand& command, IRenderer3d& renderer) { renderer.submit(command, child_, 0); }
} // namespace ce
