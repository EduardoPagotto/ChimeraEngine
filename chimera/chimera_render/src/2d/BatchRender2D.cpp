#include "chimera_render/2d/BatchRender2D.hpp"

namespace ce {

    BatchRender2D::BatchRender2D() {
        index_count_ = 0;
        this->init();
    }

    BatchRender2D::~BatchRender2D() {
        ibo_.reset();
        p_vao_.reset(); // VBO's deted here!!!
    }

    void BatchRender2D::init() {

        p_vao_ = std::make_shared<VertexArray>();
        p_vbo_ = std::make_shared<VertexBuffer>(BufferType::DYNAMIC);

        p_vao_->push(p_vbo_);

        p_vao_->bind();
        p_vbo_->bind();

        BufferLayout layout;
        layout.push<float>(3, false);
        layout.push<float>(2, false);
        layout.push<float>(1, false);
        layout.push<float>(4, false);

        p_vbo_->set_layout(layout);
        p_vbo_->set_data(nullptr, renderer_buffer_size);
        p_vbo_->unbind();

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

        ibo_ = std::make_shared<IndexBuffer>(indices, renderer_indices_size);

        p_vao_->unbind();
    }

    void BatchRender2D::begin(std::shared_ptr<Camera> camera) {

        this->camera_ = camera;

        p_vbo_->bind();
        this->buffer_ = (VertexDataSimple*)p_vbo_->map();
    }

    float BatchRender2D::submit_texture(std::shared_ptr<Texture> texture) {
        float result = 0.0F;
        bool found = false;
        for (uint i = 0; i < textures_.size(); i++) {
            if (textures_[i] == texture) {
                result = (float)(i + 1);
                found = true;
                break;
            }
        }

        if (!found) {
            if (textures_.size() >= renderer_max_texture) {
                end();          // End();
                flush();        // Present();
                begin(camera_); // Begin();
            }
            textures_.push_back(texture);
            result = (float)(textures_.size());
        }
        return result;
    }

    void BatchRender2D::submit(IRenderable2D* renderable) {

        const Prop2D& prop = ((Renderable2D*)renderable)->get_prop(); // Perigo
        const glm::vec3& position = prop.position;
        const glm::vec2& size = prop.size;
        const glm::vec4& color = prop.color;
        const std::vector<glm::vec2>& uv = prop.uv;

        float texture_slot = 0.0F; // float ts = 0.0f;
        if (prop.texture != nullptr) {
            texture_slot = this->submit_texture(prop.texture);
        }

        buffer_->point =
            stack_.multipl_vec3(position); //  glm::vec3(transformationStack.back() * glm::vec4(position, 1.0f));
        buffer_->uv = uv[0];
        buffer_->tid = texture_slot;
        buffer_->color = color;
        buffer_++;

        buffer_->point = stack_.multipl_vec3(glm::vec3(position.x, position.y + size.y, position.z));
        buffer_->uv = uv[1];
        buffer_->tid = texture_slot;
        buffer_->color = color;
        buffer_++;

        buffer_->point = stack_.multipl_vec3(glm::vec3(position.x + size.x, position.y + size.y, position.z));
        buffer_->uv = uv[2];
        buffer_->tid = texture_slot;
        buffer_->color = color;
        buffer_++;

        buffer_->point = stack_.multipl_vec3(glm::vec3(position.x + size.x, position.y, position.z));
        buffer_->uv = uv[3];
        buffer_->tid = texture_slot;
        buffer_->color = color;
        buffer_++;

        index_count_ += 6;
    }

    void BatchRender2D::draw_string(std::shared_ptr<Font> font, const std::string& text, const glm::vec3& pos,
                                    const glm::vec4& color) {

        // float textureSlot = 0.0F; // float ts = 0.0f;
        const float texture_slot = this->submit_texture(font->texture);

        const glm::vec2& scale = font->scale;
        float x = pos.x;

        for (auto c : text) {

            if (c < font->glyphs.size()) {

                const Font::GlyphData glyph = font->glyphs[c];

                // FIXME: encontrar o kering!!!!!!
                // if (i > 0) {
                //     float kering = texture_glyph_get_kering(glyph, text[1 - 1]);
                //     x += kering * scale.x;
                // }

                const float x0 = x + (static_cast<float>(glyph.offset.x) * scale.x);
                const float x1 = x0 + (static_cast<float>(glyph.size.x) * scale.x);
                const float y1 = pos.y + (static_cast<float>(glyph.offset.y) * scale.y);
                const float y0 = y1 - (static_cast<float>(glyph.size.y) * scale.y);

                const float u0 = glyph.square.x;
                const float v0 = glyph.square.y;
                const float u1 = glyph.square.w;
                const float v1 = glyph.square.h;

                buffer_->point = stack_.multipl_vec3(glm::vec3(x0, y0, 0.0F));
                buffer_->uv = glm::vec2(u0, v0);
                buffer_->tid = texture_slot;
                buffer_->color = color;
                buffer_++;

                buffer_->point = stack_.multipl_vec3(glm::vec3(x0, y1, 0.0F));
                buffer_->uv = glm::vec2(u0, v1); // glm::vec2(u0, v1);
                buffer_->tid = texture_slot;
                buffer_->color = color;
                buffer_++;

                buffer_->point = stack_.multipl_vec3(glm::vec3(x1, y1, 0.0F));
                buffer_->uv = glm::vec2(u1, v1);
                buffer_->tid = texture_slot;
                buffer_->color = color;
                buffer_++;

                buffer_->point = stack_.multipl_vec3(glm::vec3(x1, y0, 0.0F));
                buffer_->uv = glm::vec2(u1, v0);
                buffer_->tid = texture_slot;
                buffer_->color = color;
                buffer_++;

                index_count_ += 6;

                x += static_cast<float>(glyph.advance) * scale.x;
            }
        }
    }

    void BatchRender2D::end() {
        p_vbo_->unmap();
        p_vbo_->unbind();
    }

    void BatchRender2D::flush() {

        BinaryStateEnable blend(GL_BLEND, GL_TRUE);
        // BinaryStateEnable depth(GL_DEPTH_TEST, GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        BinaryStateEnable cull(GL_CULL_FACE, GL_FALSE);
        // bind shader and uniforms from model
        glUseProgram(render_comando_->shader->get_id());
        for (const auto& kv : render_comando_->uniforms) {
            render_comando_->shader->set_uniform_u(kv.first.c_str(), kv.second);
        }

        for (auto i = 0; i < textures_.size(); i++) {
            textures_[i]->bind(i);
        }

        p_vao_->bind();
        ibo_->bind();

        glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr);

        ibo_->unbind();
        p_vao_->unbind();
        index_count_ = 0;
        textures_.clear();
        glUseProgram(0);
    }

} // namespace ce
