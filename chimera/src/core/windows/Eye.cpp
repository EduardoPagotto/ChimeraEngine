#include "chimera/core/windows/Eye.hpp"
#include "chimera/core/Exception.hpp"

namespace Chimera {

Eye::Eye(const uint32_t& posX, const uint32_t& posY, const uint32_t& width, uint32_t height, const Shader& shader)
    : posX(posX), posY(posY), width(width), height(height), shader(shader), frameBuffer(nullptr), vbo(nullptr) {

    frameBuffer = new FrameBuffer(width, height);
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Render Framebuffer position(%d x %d) size(%d x %d)", posX, posY, width, height);

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

Eye::~Eye() {
    delete frameBuffer;
    delete vbo;
}

void Eye::bind() { frameBuffer->bind(); }

void Eye::unbind() { frameBuffer->unbind(); }

void Eye::renderText() {
    // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glViewport(posX, posY, width, height);

    // Clear the screen
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.enable();

    // Bind our texture in Texture Unit 0
    frameBuffer->getTexture()->bind(0);

    // Set our "renderedTexture" sampler to user Texture Unit 0
    shader.setUniform("renderedTexture", 0);

    vbo->bind();
    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    vbo->unbind();
    shader.disable();
}
} // namespace Chimera