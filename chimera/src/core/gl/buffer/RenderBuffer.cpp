#include "chimera/core/gl/buffer/RenderBuffer.hpp"

namespace ce {

    RenderBuffer::RenderBuffer(const uint32_t& posX, const uint32_t& posY, FrameBuffer* fb,
                               std::shared_ptr<Shader> shader)
        : posX(posX), posY(posY), shader(shader), vbo(nullptr), frameBuffer(fb) {

        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Render Framebuffer position(%d x %d) size(%d x %d)", posX, posY,
                     fb->getWidth(), fb->getHeight());

        // The fullscreen quad's FBO
        const glm::vec3 quad[] = {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f),
                                  glm::vec3(-1.0f, 1.0f, 0.0f),  glm::vec3(-1.0f, 1.0f, 0.0f),
                                  glm::vec3(1.0f, -1.0f, 0.0f),  glm::vec3(1.0f, 1.0f, 0.0f)};
        BufferLayout b;
        b.Push<float>(3, false);

        vbo = new VertexBuffer(BufferType::STATIC);
        vbo->bind();
        vbo->setLayout(b);
        vbo->setData(quad, 6);
        vbo->unbind();
    }

    void RenderBuffer::bind() {
        frameBuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
    }

    void RenderBuffer::render() {
        // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glViewport(posX, posY, frameBuffer->getWidth(), frameBuffer->getHeight());

        glUseProgram(shader->getID());

        // Bind our texture in Texture Unit 0
        frameBuffer->getColorAttachemnt(0)->bind(0); // getTexture()->bind(0);

        // Set our "renderedTexture" sampler to user Texture Unit 0
        shader->setUniformU("renderedTexture", Uniform(0));

        vbo->bind();
        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
        vbo->unbind();
        glUseProgram(0);
    }
} // namespace ce
