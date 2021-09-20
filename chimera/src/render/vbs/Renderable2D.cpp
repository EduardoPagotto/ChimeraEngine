#include "chimera/render/vbs/Renderable2D.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Renderable2D::Renderable2D(glm::vec3 pos, glm::vec2 size, glm::vec4 color, Shader* shader)
    : pos(pos), size(size), color(color), shader(shader) {
    vao = new Core::VertexArray();

    uint32_t indice[] = {0, 1, 2, 2, 3, 0};
    ibo = new Core::IndexBuffer(indice, 6);

    GLfloat vertices[] = {0.0,    0.0,    0.0,  //
                          0.0,    size.y, 0.0,  //
                          size.x, size.y, 0.0,  //
                          size.x, 0.0,    0.0}; //

    GLfloat colors[] = {color.x, color.y, color.z, color.w,  //
                        color.x, color.y, color.z, color.w,  //
                        color.x, color.y, color.z, color.w,  //
                        color.x, color.y, color.z, color.w}; //

    vao->addBuffer(new Core::VertexBuffer(vertices, 4 * 3, vertexDataSingleComponentes(0, 3, sizeof(GL_FLOAT))));
    vao->addBuffer(new Core::VertexBuffer(colors, 4 * 4, vertexDataSingleComponentes(1, 4, sizeof(GL_FLOAT))));
}

Renderable2D::~Renderable2D() { // PAREI AQUI : 12:03 de https://www.youtube.com/watch?v=o_0bdD_zUas
    delete vao;
    delete ibo;
}

//----

void SimpleRender2D::submit(IRenderable2D* renderable) { renderQueue.push_back(renderable); }

void SimpleRender2D::flush() {
    while (!renderQueue.empty()) {
        auto r = renderQueue.front();

        r->getVao()->bind();
        r->getIbo()->bind();

        glm::mat4 modelMatrix(1.0f);
        glm::mat4 mm = glm::translate(modelMatrix, r->getPosition());
        r->getShader()->setUniformMatrix4fv("ml_matrix", 1, false, glm::value_ptr(mm));

        glDrawElements(GL_TRIANGLES, r->getIbo()->getCount(), GL_UNSIGNED_INT, 0);

        r->getIbo()->unbind();
        r->getVao()->unbind();

        renderQueue.pop_front();
    }
}

} // namespace Chimera