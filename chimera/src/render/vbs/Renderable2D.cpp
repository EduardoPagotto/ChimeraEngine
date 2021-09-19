#include "chimera/render/vbs/Renderable2D.hpp"

namespace Chimera {

Renderable2D::Renderable2D(glm::vec3 pos, glm::vec3 size, glm::vec4 color) : pos(pos), size(size), color(color) {
    vao = new Core::VertexArray();

    uint32_t indice[] = {0, 1, 2, 2, 3, 0};
    ibo = new Core::IndexBuffer(indice, 6);

    GLfloat vertices[] = {0.0,   0.0,   0.0,  //
                          0.0,   pos.y, 0.0,  //
                          pos.x, pos.y, 0.0,  //
                          pos.x, 0.0,   0.0}; //

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

} // namespace Chimera