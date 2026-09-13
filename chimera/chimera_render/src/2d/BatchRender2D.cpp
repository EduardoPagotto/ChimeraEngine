#include "chimera/render/2d/BatchRender2D.hpp"

namespace ce {

    BatchRender2D::BatchRender2D() {
        indexCount = 0;
        this->init();
    }

    BatchRender2D::~BatchRender2D() {
        ibo.reset();
        pVao.reset(); // VBO's deted here!!!
    }

    void BatchRender2D::init() {

        pVao = std::make_shared<VertexArray>();
        pVbo = std::make_shared<VertexBuffer>(BufferType::DYNAMIC);

        pVao->push(pVbo);

        pVao->bind();
        pVbo->bind();

        BufferLayout layout;
        layout.Push<float>(3, false);
        layout.Push<float>(2, false);
        layout.Push<float>(1, false);
        layout.Push<float>(4, false);

        pVbo->setLayout(layout);
        pVbo->setData(nullptr, renderer_buffer_size);
        pVbo->unbind();

        uint32_t indices[renderer_indices_size];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < renderer_indices_size; i += 6) {
            indices[i] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }

        ibo = std::make_shared<IndexBuffer>(indices, renderer_indices_size);

        pVao->unbind();
    }

    void BatchRender2D::begin(std::shared_ptr<Camera> camera) {

        this->camera = camera;

        pVbo->bind();
        this->buffer = (VertexDataSimple*)pVbo->map();
    }

    float BatchRender2D::submitTexture(std::shared_ptr<Texture> texture) {
        float result = 0.0f;
        bool found = false;
        for (uint i = 0; i < textures.size(); i++) {
            if (textures[i] == texture) {
                result = (float)(i + 1);
                found = true;
                break;
            }
        }

        if (!found) {
            if (textures.size() >= renderer_max_texture) {
                end();         // End();
                flush();       // Present();
                begin(camera); // Begin();
            }
            textures.push_back(texture);
            result = (float)(textures.size());
        }
        return result;
    }

    void BatchRender2D::submit(IRenderable2D* renderable) {

        const Prop2D& prop = ((Renderable2D*)renderable)->getProp(); // Perigo
        const glm::vec3& position = prop.position;
        const glm::vec2& size = prop.size;
        const glm::vec4& color = prop.color;
        const std::vector<glm::vec2>& uv = prop.uv;

        float textureSlot = 0.0f; // float ts = 0.0f;
        if (prop.texture != nullptr)
            textureSlot = this->submitTexture(prop.texture);

        buffer->point =
            stack.multiplVec3(position); //  glm::vec3(transformationStack.back() * glm::vec4(position, 1.0f));
        buffer->uv = uv[0];
        buffer->tid = textureSlot;
        buffer->color = color;
        buffer++;

        buffer->point = stack.multiplVec3(glm::vec3(position.x, position.y + size.y, position.z));
        buffer->uv = uv[1];
        buffer->tid = textureSlot;
        buffer->color = color;
        buffer++;

        buffer->point = stack.multiplVec3(glm::vec3(position.x + size.x, position.y + size.y, position.z));
        buffer->uv = uv[2];
        buffer->tid = textureSlot;
        buffer->color = color;
        buffer++;

        buffer->point = stack.multiplVec3(glm::vec3(position.x + size.x, position.y, position.z));
        buffer->uv = uv[3];
        buffer->tid = textureSlot;
        buffer->color = color;
        buffer++;

        indexCount += 6;
    }

    void BatchRender2D::drawString(std::shared_ptr<Font> font, const std::string& text, const glm::vec3& pos,
                                   const glm::vec4& color) {

        float textureSlot = 0.0f; // float ts = 0.0f;
        if (font->getTexture() != nullptr)
            textureSlot = this->submitTexture(font->getTexture());

        const glm::vec2& scale = font->getScale();
        float x = pos.x;

        for (int i = 0; i < text.size(); i++) {

            uint16_t c = text[i];
            if (GlyphData* glyph = font->glyphs[c]; glyph != nullptr) {

                // FIXME: encontrar o kering!!!!!!
                // if (i > 0) {
                //     float kering = texture_glyph_get_kering(glyph, text[1 - 1]);
                //     x += kering * scale.x;
                // }

                float x0 = x + glyph->offset.x * scale.x;
                float x1 = x0 + glyph->size.x * scale.x;
                float y1 = pos.y + glyph->offset.y * scale.y;
                float y0 = y1 - glyph->size.y * scale.y;

                float u0 = glyph->square.x;
                float v0 = glyph->square.y;
                float u1 = glyph->square.w;
                float v1 = glyph->square.h;

                buffer->point = stack.multiplVec3(glm::vec3(x0, y0, 0.0f));
                buffer->uv = glm::vec2(u0, v0);
                buffer->tid = textureSlot;
                buffer->color = color;
                buffer++;

                buffer->point = stack.multiplVec3(glm::vec3(x0, y1, 0.0f));
                buffer->uv = glm::vec2(u0, v1); // glm::vec2(u0, v1);
                buffer->tid = textureSlot;
                buffer->color = color;
                buffer++;

                buffer->point = stack.multiplVec3(glm::vec3(x1, y1, 0.0f));
                buffer->uv = glm::vec2(u1, v1);
                buffer->tid = textureSlot;
                buffer->color = color;
                buffer++;

                buffer->point = stack.multiplVec3(glm::vec3(x1, y0, 0.0f));
                buffer->uv = glm::vec2(u1, v0);
                buffer->tid = textureSlot;
                buffer->color = color;
                buffer++;

                indexCount += 6;

                x += glyph->advance * scale.x;
            }
        }
    }

    void BatchRender2D::end() {
        pVbo->unmap();
        pVbo->unbind();
    }

    void BatchRender2D::flush() {

        BinaryStateEnable blend(GL_BLEND, GL_TRUE);
        // BinaryStateEnable depth(GL_DEPTH_TEST, GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        BinaryStateEnable cull(GL_CULL_FACE, GL_FALSE);
        // bind shader and uniforms from model
        glUseProgram(renderComando->shader->getID());
        for (const auto& kv : renderComando->uniforms)
            renderComando->shader->setUniformU(kv.first.c_str(), kv.second);

        for (uint8_t i = 0; i < textures.size(); i++)
            textures[i]->bind(i);

        pVao->bind();
        ibo->bind();

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

        ibo->unbind();
        pVao->unbind();
        indexCount = 0;
        textures.clear();
        glUseProgram(0);
    }

} // namespace ce
