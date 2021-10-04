#include "chimera/core/windows/Eye.hpp"
#include "chimera/core/Exception.hpp"
//#include <glm/gtc/matrix_transform.hpp>

namespace Chimera::Core {

Eye::Eye(const unsigned short& _indexEye, const int& _w, const int& _h, Shader* _pShader) {

    frameBuffer = nullptr;

    this->quad_vertexbuffer = 0;
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
    glDeleteBuffers(1, &quad_vertexbuffer);
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

void Eye::bind() { frameBuffer->bind(); } //  ????

void Eye::unbind() { frameBuffer->unbind(); }

glm::mat4 Eye::getPerspectiveProjectionMatrix(ViewPoint* vp) {
    glViewport(0, 0, fbTexGeo.w, fbTexGeo.h);
    return glm::perspective(vp->fov, (GLfloat)(float)fbTexGeo.w / (float)fbTexGeo.h, vp->near, vp->far);
}

glm::mat4 Eye::getOrthoProjectionMatrix() {
    return glm::ortho(0.0f, static_cast<GLfloat>(fbTexGeo.w), 0.0f, static_cast<GLfloat>(fbTexGeo.h));
}

void Eye::createSquare() {

    // The fullscreen quad's FBO
    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    };

    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

    texID = pShader->getUniformLocation("renderedTexture");
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

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void*)0  // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

    glDisableVertexAttribArray(0);

    pShader->disable();
}

} // namespace Chimera::Core