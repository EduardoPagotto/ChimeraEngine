#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/3d/RenderableIBO.hpp"
#include "chimera/render/VertexData.hpp"

namespace Chimera {

Renderable3D::Renderable3D(Mesh* mesh) {
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

Renderable3D::~Renderable3D() {

    delete vao;
    vao = nullptr;

    delete child;
    child = nullptr;
}

void Renderable3D::submit(RenderCommand& command, IRenderer3d& renderer) {
    renderer.submit(command);
    command.renderable = child;
    renderer.submit(command);
}

void Renderable3D::draw(const bool& logData) {

    if (logData)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Renderable3D draw"); // TODO: ver o que fazer
}

} // namespace Chimera