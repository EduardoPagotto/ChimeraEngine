#ifndef __CHIMERA_CANVAS_GL__HPP
#define __CHIMERA_CANVAS_GL__HPP

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>

#include "chimera/core/Canvas.hpp"

namespace Chimera {

class CanvasGL : public Canvas {
  public:
    CanvasGL(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~CanvasGL();

    virtual void before() override;
    virtual void after() override;
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;

    virtual glm::mat4 getPerspectiveProjectionMatrix(const float& _fov, const float& _near, const float& _far,
                                                     int _eye);
    virtual glm::mat4 getOrthoProjectionMatrix(int eyeIndex);
    virtual int getTotEyes() { return 1; }

    virtual void initGL();
    virtual void afterStart();
    virtual void restoreMatrix();
    std::string getVersaoOpenGL();

  protected:
    SDL_GLContext context;
};
} // namespace Chimera

#endif
