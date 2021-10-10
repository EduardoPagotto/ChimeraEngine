#include "chimera/core/windows/Eye.hpp"
#include "chimera/core/Exception.hpp"
//#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

Eye::Eye(const unsigned short& _indexEye, const int& _w, const int& _h, Shader* _pShader) {

    frameBuffer = nullptr;
    this->pShader = _pShader;

    this->fbTexGeo.w = next_pow2(_w);
    this->fbTexGeo.h = next_pow2(_h);
    this->fbTexGeo.x = (_indexEye == 0) ? 0 : this->fbTexGeo.w;
    this->fbTexGeo.y = 0;

    frameBuffer = new FrameBuffer(fbTexGeo.w, fbTexGeo.h);
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "created frame buffer render target: %dx%d", fbTexGeo.w, fbTexGeo.h);

    this->createSquare();

    if (_indexEye == 0)
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Left Eye Setup pos:%d size:%d", this->fbTexGeo.x, this->fbTexGeo.w);
    else
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Right Eye Setup pos:%d size:%d", this->fbTexGeo.x, this->fbTexGeo.w);
}

Eye::~Eye() {
    delete frameBuffer;
    delete vbo;
}

unsigned int Eye::next_pow2(unsigned int x) {
    x -= 1;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

void Eye::bind() { frameBuffer->bind(); }

void Eye::unbind() { frameBuffer->unbind(); }

void Eye::createSquare() {

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
    texID = pShader->getUniform("renderedTexture");
}

void Eye::displayTexture() {

    // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glViewport(this->fbTexGeo.x, this->fbTexGeo.y, fbTexGeo.w, fbTexGeo.h);

    // Clear the screen
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pShader->enable();

    // Bind our texture in Texture Unit 0
    frameBuffer->getTexture()->bind(0);

    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(texID, 0);

    vbo->bind();

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

    vbo->unbind();

    pShader->disable();
}

} // namespace Chimera