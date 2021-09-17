#ifndef __CHIMERA_EYE_HDM__HPP
#define __CHIMERA_EYE_HDM__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/ViewPoint.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace Chimera::Core {

class Eye {
  public:
    Eye(const unsigned short& _indexEye, const int& _w, const int& _h, Shader* _pShader);
    virtual ~Eye();
    void displayTexture();
    void bind();
    void unbind();
    glm::mat4 getPerspectiveProjectionMatrix(ViewPoint* vp);
    glm::mat4 getOrthoProjectionMatrix();

  private:
    unsigned int posInitW;
    GLuint fbo;
    GLuint fb_tex;
    GLuint fb_depth;
    GLuint texID;
    GLuint quad_vertexbuffer;
    SDL_Rect fbTexGeo;
    Shader* pShader;

    void createFBO();
    void createSquare();
    unsigned int next_pow2(unsigned int x);
};

} // namespace Chimera::Core

#endif
