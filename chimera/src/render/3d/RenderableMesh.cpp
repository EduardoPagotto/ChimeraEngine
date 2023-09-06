#include "chimera/render/3d/RenderableMesh.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderableIBO.hpp"

namespace Chimera {

RenderableMesh::RenderableMesh(Mesh* mesh) : Renderable3D() {

    Mesh temp;
    meshReindex(*mesh, temp);

    // Create VAO, VBO and IBO
    vao = new VertexArray();
    vao->bind();

    VertexBuffer* vbo = new VertexBuffer(BufferType::STATIC);
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

    aabb.setBoundary(min, max);
    IndexBuffer* ibo = new IndexBuffer((uint32_t*)&temp.iFace[0], temp.iFace.size() * 3);
    totIndex = ibo->getSize();
    child = new RenderableIBO(vao, ibo, AABB(min, max));
}

RenderableMesh::~RenderableMesh() {
    delete child;
    child = nullptr;
}

void RenderableMesh::submit(RenderCommand& command, IRenderer3d& renderer) { renderer.submit(command, child, 0); }
} // namespace Chimera