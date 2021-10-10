#ifndef __CHIMERA_EYE_HDM__HPP
#define __CHIMERA_EYE_HDM__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/buffers/FrameBuffer.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace Chimera {

class Eye {
  public:
    Eye(const unsigned short& _indexEye, const int& _w, const int& _h, Shader* _pShader);
    virtual ~Eye();
    void displayTexture();
    void bind();
    void unbind();
    glm::mat4 getOrthoProjectionMatrix();

    SDL_Rect fbTexGeo;

  private:
    unsigned int posInitW;
    GLuint texID;
    VertexBuffer* vbo;
    Shader* pShader;
    FrameBuffer* frameBuffer;

    void createSquare();
    unsigned int next_pow2(unsigned int x);
};
} // namespace Chimera
#endif
