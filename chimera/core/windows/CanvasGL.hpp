#ifndef __CHIMERA_CANVAS_GL__HPP
#define __CHIMERA_CANVAS_GL__HPP

#include "Canvas.hpp"

namespace Chimera {

class CanvasGL : public Canvas {
  public:
    CanvasGL(const std::string& _title, int _width, int _height, bool _fullScreen = false);
    virtual ~CanvasGL();

    virtual void before(const unsigned short& _indexEye = 0) override;
    virtual void after(const unsigned short& _indexEye = 0) override;
    virtual void toggleFullScreen() override;
    virtual void reshape(int _width, int _height) override;

    virtual void swapWindow();
    virtual glm::mat4 getOrthoProjectionMatrix(int eyeIndex);
    virtual int getTotEyes() { return 1; }
    virtual uint32_t* getPixels() override { return nullptr; }

    virtual void afterStart() override;
    std::string getVersaoOpenGL();

  protected:
    SDL_GLContext context;
};
} // namespace Chimera

#endif
