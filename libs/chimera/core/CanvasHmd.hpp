#ifndef __CHIMERA_CANVAS_HDM__HPP
#define __CHIMERA_CANVAS_HDM__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Eye.hpp"

namespace Chimera {

class CanvasHmd : public CanvasGL {
  public:
    CanvasHmd(const std::string& _title, int _width, int _height);
    virtual ~CanvasHmd();

    virtual void before() override;
    virtual void after() override;

    virtual glm::mat4 getPerspectiveProjectionMatrix(ViewPoint* vp, int _eye) override;

    virtual glm::mat4 getOrthoProjectionMatrix(int eyeIndex) override;
    virtual int getTotEyes() {
        // FIXME: voltar para 2 quando esterioscopia for implementada
        return 1;
    }

  private:
    Eye* pLeft;
    Shader* pShader;
};
} // namespace Chimera

#endif