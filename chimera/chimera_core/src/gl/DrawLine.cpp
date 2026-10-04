#include "chimera_core/gl/DrawLine.hpp"

namespace ce {

    // ref: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=11517

    void DrawLine::create(std::shared_ptr<Shader> shader, const uint32_t& size_buffer) noexcept {

        this->shader_ = shader;
        vao_ = std::make_shared<VertexArray>();
        vbo_ = std::make_shared<VertexBuffer>(BufferType::STREAM);

        vao_->push(vbo_);

        vao_->bind();
        vbo_->bind();

        BufferLayout layout;
        layout.push<float>(3, false); // point
        layout.push<float>(3, false); // color

        vbo_->set_layout(layout);
        vbo_->set_data(nullptr, size_buffer);
    }

    void DrawLine::destroy() noexcept {
        vao_.reset();
        shader_.reset();
        points_.clear();
    }

    void DrawLine::render(MapUniform& uniforms_queue) noexcept {
        glUseProgram(shader_->get_id());

        for (const auto& kv : uniforms_queue)
            shader_->set_uniform_u(kv.first.c_str(), kv.second);

        vao_->bind();
        vbo_->bind();

        vbo_->set_sub_data(&points_[0], 0, points_.size()); // load tata dynamic

        glDrawArrays(GL_LINES, 0, points_.size());

        vbo_->unbind();
        vao_->unbind();

        glUseProgram(0);

        points_.clear();
    }

    void DrawLine::add_aabb(const AABB& aabb, const glm::vec3& color) noexcept {
        const std::array<glm::vec3, 8>& v = aabb.getAllVertex();
        add(v[0], v[1], color);
        add(v[2], v[3], color);
        add(v[4], v[5], color);
        add(v[6], v[7], color);
        add(v[0], v[2], color);
        add(v[1], v[3], color);
        add(v[4], v[6], color);
        add(v[5], v[7], color);
        add(v[0], v[4], color);
        add(v[1], v[5], color);
        add(v[2], v[6], color);
        add(v[3], v[7], color);
    }
} // namespace ce
