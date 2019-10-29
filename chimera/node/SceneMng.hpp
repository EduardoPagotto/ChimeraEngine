#ifndef __CHIMERA_SCENE_MNG__HPP
#define __CHIMERA_SCENE_MNG__HPP

#include "Node.hpp"
#include "RenderVisitor.hpp"
#include "chimera/core/CanvasGL.hpp"

namespace Chimera {

/**
 * Class SceneMng
 *  @author <a href="mailto:edupagotto@gmail.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class SceneMng {
  public:
    SceneMng(CanvasGL* _pCanvas, Node* _pRoot);
    virtual ~SceneMng() noexcept;
    void init();
    void render();
    inline Node* getRoot() { return pRoot; }
    inline CanvasGL* getCanvas() { return pCanvas; }
    inline void origemDesenho(Transform* _pTransform) { renderV.pTransform = _pTransform; }

  private:
    Node* pRoot;
    CanvasGL* pCanvas;
    RenderVisitor renderV;
};
} // namespace Chimera
#endif
