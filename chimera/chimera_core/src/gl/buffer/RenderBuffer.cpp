#include "chimera_core/gl/buffer/RenderBuffer.hpp"

namespace ce {

    RenderBuffer::RenderBuffer(const uint32_t& pos_x, const uint32_t& pos_y, std::shared_ptr<FrameBuffer> fb,
                               std::shared_ptr<Shader> shader)
        : pos_x_(pos_x), pos_y_(pos_y), shader_(shader), frame_buffer_(fb) {

        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Render Framebuffer position(%d x %d) size(%d x %d)", pos_x, pos_y,
                     fb->width(), fb->height());

        // The fullscreen quad's FBO
        const glm::vec3 quad[] = {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f),
                                  glm::vec3(-1.0f, 1.0f, 0.0f),  glm::vec3(-1.0f, 1.0f, 0.0f),
                                  glm::vec3(1.0f, -1.0f, 0.0f),  glm::vec3(1.0f, 1.0f, 0.0f)};
        BufferLayout b;
        b.push<float>(3, false);

        vao_ = std::make_shared<VertexArray>();
        vao_->bind();

        vbo_ = std::make_shared<VertexBuffer>(BufferType::STATIC);
        vbo_->bind();
        vbo_->set_layout(b);
        vbo_->set_data(quad, 6);
        vbo_->unbind();

        vao_->push(vbo_);
        vao_->unbind();
    }

    void RenderBuffer::bind() {
        frame_buffer_->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
    }

    void RenderBuffer::render() {
        // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glViewport(pos_x_, pos_y_, frame_buffer_->width(), frame_buffer_->height());

        glUseProgram(shader_->get_id());

        // Bind our texture in Texture Unit 0
        frame_buffer_->get_color_attachemnt(0)->bind(0); // getTexture()->bind(0);

        // Set our "renderedTexture" sampler to user Texture Unit 0
        shader_->set_uniform_u("renderedTexture", Uniform(0));

        vao_->bind();

        // vbo->bind();
        //  Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
        // vbo->unbind();
        vao_->unbind();
        glUseProgram(0);
    }
} // namespace ce
