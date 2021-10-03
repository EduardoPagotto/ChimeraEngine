#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/core/VertexData.hpp"

namespace Chimera {

BatchRender2D::BatchRender2D() {
    indexCount = 0;
    this->init();
}

BatchRender2D::~BatchRender2D() {
    delete ibo;
    delete pVao;
    delete pVbo;
}

void BatchRender2D::init() {

    pVao = new Core::VertexArray();
    pVbo = new Core::VertexBuffer(Core::BufferType::DYNAMIC);

    pVao->push(pVbo);

    pVao->bind();
    pVbo->bind();

    BufferLayout layout;
    layout.push(3, GL_FLOAT, sizeof(float), false);
    layout.push(2, GL_FLOAT, sizeof(float), false);
    layout.push(1, GL_FLOAT, sizeof(float), false);
    layout.push(4, GL_FLOAT, sizeof(float), false);

    pVbo->setLayout(layout);
    pVbo->setData(nullptr, RENDERER_BUFFER_SIZE);
    pVbo->unbind();

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

    pVao->unbind();
}

void BatchRender2D::begin() {

    pVbo->bind();
    this->buffer = (VertexDataSimple*)pVbo->map();
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

            if (textureSlots.size() > RENDERER_MAX_TEXTURE) {
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

    const GLuint tid = font->getTextureID();

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

            if (textureSlots.size() > RENDERER_MAX_TEXTURE) {
                end();
                flush();
                begin();
            }

            textureSlots.push_back(tid);
            ts = (float)(textureSlots.size());
        }
    }

    const glm::vec2& scale = font->getScale();
    float x = pos.x;

    for (int i = 0; i < text.size(); i++) {

        uint16_t c = text[i];
        GlyphData* glyph = font->glyphs[c];

        if (glyph != nullptr) {

            // FIXME: encontrar o kering!!!!!!
            // if (i > 0) {
            //     float kering = texture_glyph_get_kering(glyph, text[1 - 1]);
            //     x += kering / scale.x;
            // }

            float x0 = x + glyph->offset.x / scale.x;
            float x1 = x0 + glyph->size.x / scale.x;
            float y1 = pos.y + glyph->offset.y / scale.y;
            float y0 = y1 - glyph->size.y / scale.y;

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

            x += glyph->advance / scale.x;
        }
    }
}

void BatchRender2D::end() {
    pVbo->unmap();
    pVbo->unbind();
}

void BatchRender2D::flush() {

    for (int i = 0; i < textureSlots.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureSlots[i]);
    }

    pVao->bind();
    ibo->bind();

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    ibo->unbind();
    pVao->unbind();

    indexCount = 0;
    textureSlots.clear();
}

} // namespace Chimera