#ifndef __CHIMERA_EYE_HDM__HPP
#define __CHIMERA_EYE_HDM__HPP

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>

#include "chimera/core/Logger.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/ViewPoint.hpp"

namespace Chimera {

struct EyeSize {
    unsigned int w;
    unsigned int h;
};

class Eye {
  public:
    Eye(const int& _w, const int& _h, Shader* _pShader);
    virtual ~Eye();

    void displayTexture();
    void begin();
    void end();
    glm::mat4 getPerspectiveProjectionMatrix(ViewPoint* vp);
    glm::mat4 getOrthoProjectionMatrix();

  private:
    GLuint fbo;
    GLuint fb_tex;
    GLuint fb_depth;
    GLuint texID;
    GLuint timeID;
    GLuint quad_vertexbuffer;
    EyeSize fbTexSize;
    Logger* log;
    Shader* pShader;

    void createFBO();
    void createSquare();
    unsigned int next_pow2(unsigned int x);
};

} // namespace Chimera

#endif
