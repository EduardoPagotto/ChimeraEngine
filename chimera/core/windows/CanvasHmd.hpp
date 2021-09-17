#ifndef __CHIMERA_CANVAS_HDM__HPP
#define __CHIMERA_CANVAS_HDM__HPP

#include "CanvasGL.hpp"
#include "Eye.hpp"

namespace Chimera::Core {

class CanvasHmd : public CanvasGL {
  public:
    CanvasHmd(const std::string& _title, int _width, int _height);
    virtual ~CanvasHmd();

    virtual void before(const unsigned short& _indexEye = 0);
    virtual void after(const unsigned short& _indexEye = 0);
    virtual void swapWindow();

    virtual void calcPerspectiveProjectionView(const unsigned short& _indexEye, ViewPoint* vp, glm::mat4& pView, glm::mat4& pProjection);

    virtual glm::mat4 getOrthoProjectionMatrix(int eyeIndex) override;
    virtual int getTotEyes() { return 2; }

  private:
    Eye* pLeft;
    Eye* pRight;
    Shader* pShader;
};
} // namespace Chimera::Core

#endif