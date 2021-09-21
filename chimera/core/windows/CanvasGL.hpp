#ifndef __CHIMERA_CANVAS_GL__HPP
#define __CHIMERA_CANVAS_GL__HPP

#include "Canvas.hpp"
#include "chimera/core/ViewPoint.hpp"
#include <glm/glm.hpp>

namespace Chimera::Core {

class CanvasGL : public Canvas {
  public:
    CanvasGL(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~CanvasGL();

    virtual void before(const unsigned short& _indexEye = 0);
    virtual void after(const unsigned short& _indexEye = 0);
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;

    virtual void swapWindow();
    virtual void calcPerspectiveProjectionView(const unsigned short& _indexEye, ViewPoint* vp, glm::mat4& pView, glm::mat4& pProjection);
    virtual glm::mat4 getOrthoProjectionMatrix(int eyeIndex);
    virtual int getTotEyes() { return 1; }

    virtual void afterStart();
    std::string getVersaoOpenGL();

  protected:
    SDL_GLContext context;
};
} // namespace Chimera::Core

#endif