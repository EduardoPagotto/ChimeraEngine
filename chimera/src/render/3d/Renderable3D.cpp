#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
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
    ibo = new IndexBuffer(&index[0], index.size());
}

Renderable3D::~Renderable3D() {

    delete vao;
    vao = nullptr;

    delete ibo;
    ibo = nullptr;
}

void Renderable3D::submit(RenderCommand& command, IRenderer3d& renderer) { renderer.submit(command); }

void Renderable3D::draw(const bool& logData) {
    if (ibo != nullptr) { // Desenhar o IBO
        ibo->bind();
        glDrawElements(GL_TRIANGLES, ibo->getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        if (logData == true) {
            glm::vec3 size = aabb.getSize();
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "IBO ID: %d Faces: %d AABB[%.2f, %.2f, %.2f]", ibo->getBufferID(), ibo->getSize() / 3,
                         size.x, size.y, size.z);

            aabb.debug_render();
        }

        ibo->unbind();
    }
}

} // namespace Chimera