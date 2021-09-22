#include "chimera/render/vbs/BatchRender2D.hpp"

namespace Chimera {

BatchRender2D::BatchRender2D() {
    indexCount = 0;
    this->init();
}

BatchRender2D::~BatchRender2D() {
    delete ibo;

    glDeleteBuffers(1, &vbo);
}

void BatchRender2D::init() {

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
    glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(SHADER_UV_INDEX);
    glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, BUFFER_OFFSET(12));

    glEnableVertexAttribArray(SHADER_COLOR_INDEX);
    glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, BUFFER_OFFSET(20));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t indices[RENDERER_INDICES_SIZE];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < RENDERER_INDICES_SIZE; i += 6) {
        indices[i] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }

    ibo = new Core::IndexBuffer(indices, RENDERER_INDICES_SIZE);

    glBindVertexArray(0);
}

void BatchRender2D::begin() {

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    this->buffer = (VertexDataSimple*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void BatchRender2D::submit(IRenderable2D* renderable) {

    const glm::vec3& position = renderable->getPosition();
    const glm::vec2& size = renderable->getSize();
    const glm::vec4& color = renderable->getColor();
    const std::vector<glm::vec2>& uv = renderable->getUV();

    buffer->vertex = stack.multiplVec3(position); //  glm::vec3(transformationStack.back() * glm::vec4(position, 1.0f));
    buffer->uv = uv[0];
    buffer->color = color;
    buffer++;

    buffer->vertex = stack.multiplVec3(glm::vec3(position.x, position.y + size.y, position.z));
    buffer->uv = uv[1];
    buffer->color = color;
    buffer++;

    buffer->vertex = stack.multiplVec3(glm::vec3(position.x + size.x, position.y + size.y, position.z));
    buffer->uv = uv[2];
    buffer->color = color;
    buffer++;

    buffer->vertex = stack.multiplVec3(glm::vec3(position.x + size.x, position.y, position.z));
    buffer->uv = uv[3];
    buffer->color = color;
    buffer++;

    indexCount += 6;
}

void BatchRender2D::end() {
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BatchRender2D::flush() {

    glBindVertexArray(vao);
    ibo->bind();

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    ibo->unbind();
    glBindVertexArray(0);

    indexCount = 0;
}

} // namespace Chimera