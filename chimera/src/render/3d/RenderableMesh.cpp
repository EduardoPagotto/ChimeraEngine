#include "chimera/render/3d/RenderableMesh.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderableIBO.hpp"
#include "chimera/render/VertexData.hpp"

namespace Chimera {

RenderableMesh::RenderableMesh(Mesh* mesh) {
    // Transforma Mesh em VertexData comprimindo-o
    std::vector<VertexData> renderData;
    vertexDataFromMesh(mesh, renderData);

    std::vector<uint32_t> index;
    std::vector<VertexData> vertexDataOut;
    vertexDataIndexCompile(renderData, vertexDataOut, index);

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
    vbo->setData(&vertexDataOut[0], vertexDataOut.size());
    vbo->unbind();
    vao->push(vbo);
    vao->unbind();

    glm::vec3 min, max, size;
    vertexDataIndexMinMaxSize(&vertexDataOut[0], vertexDataOut.size(), &index[0], index.size(), min, max, size);

    aabb.setBoundary(min, max);
    IndexBuffer* ibo = new IndexBuffer(&index[0], index.size());
    totIndex = ibo->getSize();
    child = new RenderableIBO(ibo, AABB(min, max));
}

RenderableMesh::~RenderableMesh() {
    delete child;
    child = nullptr;
}

void RenderableMesh::submit(RenderCommand& command, IRenderer3d& renderer) {
    if (renderer.submit(command, this) == true)
        renderer.submit(command, child);
}
} // namespace Chimera