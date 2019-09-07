#ifndef __CHIMERA_CANVAS_HDM__HPP
#define __CHIMERA_CANVAS_HDM__HPP

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>

#include "chimera/core/CanvasGL.hpp"

#include "chimera/core/Shader.hpp"

namespace Chimera {

struct CanvasSize {
    unsigned int w;
    unsigned int h;
};

class CanvasHmd : public CanvasGL {
  public:
    CanvasHmd(const std::string& _title, int _width, int _height);
    virtual ~CanvasHmd();

    virtual void before() override;
    virtual void after() override;

    virtual glm::mat4 getPerspectiveProjectionMatrix(const float& _fov, const float& _near, const float& _far,
                                                     int _eye) override;
    virtual glm::mat4 getOrthoProjectionMatrix(int eyeIndex) override;
    virtual int getTotEyes() {
        // FIXME: voltar para 2 quando esterioscopia for implementada
        return 1;
    }

  private:
    CanvasSize fbTexSize;
    GLuint fbo;
    GLuint fb_tex;
    GLuint fb_depth;

    Shader* pShader;

    GLuint texID;
    GLuint timeID;

    GLuint quad_vertexbuffer;

    void createFBO();
    void createSquare();
    void displayTexture();

    unsigned int next_pow2(unsigned int x);
};
} // namespace Chimera

#endif