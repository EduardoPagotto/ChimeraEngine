#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/core/VertexData.hpp"

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

    glEnableVertexAttribArray(SHADER_TID_INDEX);
    glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, BUFFER_OFFSET(20));

    glEnableVertexAttribArray(SHADER_COLOR_INDEX);
    glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, BUFFER_OFFSET(24));

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
    const GLuint tid = renderable->getTID();

    float ts = 0.0f;
    if (tid > 0) {
        bool found = false;
        for (int i = 0; i < textureSlots.size(); i++) {

            if (textureSlots[i] == tid) {
                ts = (float)(i + 1);
                found = true;
                break;
            }
        }

        if (!found) {

            if (textureSlots.size() > 32) {
                end();
                flush();
                begin();
            }

            textureSlots.push_back(tid);
            ts = (float)(textureSlots.size());
        }
    }

    buffer->vertex = stack.multiplVec3(position); //  glm::vec3(transformationStack.back() * glm::vec4(position, 1.0f));
    buffer->uv = uv[0];
    buffer->tid = ts;
    buffer->color = color;
    buffer++;

    buffer->vertex = stack.multiplVec3(glm::vec3(position.x, position.y + size.y, position.z));
    buffer->uv = uv[1];
    buffer->tid = ts;
    buffer->color = color;
    buffer++;

    buffer->vertex = stack.multiplVec3(glm::vec3(position.x + size.x, position.y + size.y, position.z));
    buffer->uv = uv[2];
    buffer->tid = ts;
    buffer->color = color;
    buffer++;

    buffer->vertex = stack.multiplVec3(glm::vec3(position.x + size.x, position.y, position.z));
    buffer->uv = uv[3];
    buffer->tid = ts;
    buffer->color = color;
    buffer++;

    indexCount += 6;
}

void BatchRender2D::drawString(FontAtlas* font, const std::string& text, const glm::vec3& pos, const glm::vec4& color) {

    // // https://www.youtube.com/watch?v=Rsdc6I80aFQ&t=71s
    // // 32:00

    // const glm::vec3& position = pos;
    // const glm::vec2& size = renderable->getSize();
    // const glm::vec4& color = color;
    // const std::vector<glm::vec2>& uv = renderable->getUV();
    const GLuint tid = font->getTextureID();
    // const GLuint tid = 1; // 1font->getTextureID();

    float ts = 0.0f;
    if (tid > 0) {
        bool found = false;
        for (int i = 0; i < textureSlots.size(); i++) {

            if (textureSlots[i] == tid) {
                ts = (float)(i + 1);
                found = true;
                break;
            }
        }

        if (!found) {

            if (textureSlots.size() > 32) {
                end();
                flush();
                begin();
            }

            textureSlots.push_back(tid);
            ts = (float)(textureSlots.size());
        }
    }

    float scaleX = 3.0; // 960.0f / 32.0f;
    float scaleY = 3.0; // 540.0f / 18.0f;
    float x = pos.x;

    for (int i = 0; i < text.size(); i++) {

        uint16_t c = text[i];
        GlyphData* glyph = font->glyphs[c];

        if (glyph != nullptr) {

            // FIXME: encontrar o kering!!!!!!
            // if (i > 0) {
            //     float kering = texture_glyph_get_kering(glyph, text[1 - 1]);
            //     x += kering / scaleX;
            // }

            float x0 = x + glyph->offset.x / scaleX;     //  x + glyph.offSet_x / scaleX;
            float x1 = x0 + glyph->size.x / scaleX;      // x0 + glyph.with / scaleX;
            float y1 = pos.y + glyph->offset.y / scaleY; // pos.y + glyph.offSet_y / scaleY;
            float y0 = y1 - glyph->size.y / scaleY;      // y0 - glyph.height / scaleY;

            float u0 = glyph->square.x;
            float v0 = glyph->square.y;
            float u1 = glyph->square.w;
            float v1 = glyph->square.h;

            buffer->vertex = stack.multiplVec3(glm::vec3(x0, y0, 0.0f));
            buffer->uv = glm::vec2(u0, v0);
            buffer->tid = ts;
            buffer->color = color;
            buffer++;

            buffer->vertex = stack.multiplVec3(glm::vec3(x0, y1, 0.0f));
            buffer->uv = glm::vec2(u0, v1); // glm::vec2(u0, v1);
            buffer->tid = ts;
            buffer->color = color;
            buffer++;

            buffer->vertex = stack.multiplVec3(glm::vec3(x1, y1, 0.0f));
            buffer->uv = glm::vec2(u1, v1);
            buffer->tid = ts;
            buffer->color = color;
            buffer++;

            buffer->vertex = stack.multiplVec3(glm::vec3(x1, y0, 0.0f));
            buffer->uv = glm::vec2(u1, v0);
            buffer->tid = ts;
            buffer->color = color;
            buffer++;

            indexCount += 6;

            x += glyph->advance / scaleX;
        }
    }
}

void BatchRender2D::end() {
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BatchRender2D::flush() {

    for (int i = 0; i < textureSlots.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureSlots[i]);
    }

    glBindVertexArray(vao);
    ibo->bind();

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    ibo->unbind();
    glBindVertexArray(0);

    indexCount = 0;
}

} // namespace Chimera