#ifndef __CHIMERA_SCENE_MNG__HPP
#define __CHIMERA_SCENE_MNG__HPP

#include "RenderVisitor.hpp"
#include "chimera/core/CanvasGL.hpp"
#include <vector>

namespace Chimera {

/**
 * Class SceneMng
 *  @author <a href="mailto:edupagotto@gmail.com">Eduardo Pagotto</a>
 *  @since 20130925
 */
class SceneMng : public Node {
  public:
    SceneMng();
    virtual ~SceneMng() noexcept;

    virtual void accept(class NodeVisitor* v);

    void init();
    void draw(CanvasGL* _pVideo);
    void start(CanvasGL* _pVideo);

    inline void origemDesenho(Coord* _pCoord) { renderV.pCoord = _pCoord; }

  private:
    RenderVisitor renderV;
};
} // namespace Chimera
#endif
