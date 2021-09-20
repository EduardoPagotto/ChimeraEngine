#include "chimera/render/vbs/StaticSprite.hpp"

namespace Chimera {

StaticSprite::StaticSprite(float x, float y, float width, float height, glm::vec4 color, Shader* shader)
    : Renderable2D(glm::vec3(x, y, 0), glm::vec2(width, height), color), shader(shader) {

    vao = new Core::VertexArray();

    GLfloat vertices[] = {0.0,   0.0,    0.0,  //
                          0.0,   height, 0.0,  //
                          width, height, 0.0,  //
                          width, 0.0,    0.0}; //

    GLfloat colors[] = {color.x, color.y, color.z, color.w,  //
                        color.x, color.y, color.z, color.w,  //
                        color.x, color.y, color.z, color.w,  //
                        color.x, color.y, color.z, color.w}; //

    vao->addBuffer(new Core::VertexBuffer(vertices, 4 * 3, vertexDataSingleComponentes(0, 3, sizeof(GL_FLOAT))));
    vao->addBuffer(new Core::VertexBuffer(colors, 4 * 4, vertexDataSingleComponentes(1, 4, sizeof(GL_FLOAT))));

    uint32_t indice[] = {0, 1, 2, 2, 3, 0};
    ibo = new Core::IndexBuffer(indice, 6);
}

StaticSprite::~StaticSprite() {
    delete vao;
    delete ibo;
}

} // namespace Chimera