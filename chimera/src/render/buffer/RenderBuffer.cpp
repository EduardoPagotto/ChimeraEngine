#include "chimera/render/buffer/RenderBuffer.hpp"
#include "chimera/core/Exception.hpp"

namespace Chimera {

RenderBuffer::RenderBuffer(const uint32_t& posX, const uint32_t& posY, FrameBuffer* fb, const Shader& shader)
    : posX(posX), posY(posY), shader(shader), frameBuffer(fb), vbo(nullptr) {

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Render Framebuffer position(%d x %d) size(%d x %d)", posX, posY, fb->getWidth(),
                 fb->getHeight());

    // The fullscreen quad's FBO
    const glm::vec3 quad[] = {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 0.0f),
                              glm::vec3(-1.0f, 1.0f, 0.0f),  glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)};
    BufferLayout b;
    b.push(3, GL_FLOAT, sizeof(float), false);

    vbo = new VertexBuffer(BufferType::STATIC);
    vbo->bind();
    vbo->setLayout(b);
    vbo->setData(quad, 6);
    vbo->unbind();
}

RenderBuffer::~RenderBuffer() {
    delete frameBuffer;
    delete vbo;
}

void RenderBuffer::bind() {
    frameBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
}

void RenderBuffer::unbind() { frameBuffer->unbind(); }

void RenderBuffer::render() {
    // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glViewport(posX, posY, frameBuffer->getWidth(), frameBuffer->getHeight());

    glUseProgram(shader.getID());

    // Bind our texture in Texture Unit 0
    frameBuffer->getColorAttachemnt(0)->bind(0); // getTexture()->bind(0);

    // Set our "renderedTexture" sampler to user Texture Unit 0
    shader.setUniform1i("renderedTexture", 0);

    vbo->bind();
    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    vbo->unbind();
    glUseProgram(0);
}
} // namespace Chimera