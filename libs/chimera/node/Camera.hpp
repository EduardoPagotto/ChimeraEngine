#ifndef __CHIMERA_CAMERA__HPP
#define __CHIMERA_CAMERA__HPP

#include "Node.hpp"
#include <glm/glm.hpp>

//#include "chimera/core/ViewPoint.hpp"

#include "chimera/core/TrackBall.hpp"
#include "chimera/core/TrackHead.hpp"

namespace Chimera {

class Camera : public Node {
  public:
    Camera(Node* _pNode, std::string _name);
    virtual ~Camera();

    virtual void init();
    void createTrackBall();
    void createTrackHead();

    // Inherited via Node
    virtual void accept(class NodeVisitor* v) override;

    inline ViewPoint* getViewPoint() { return &viewPoint; }
    inline TrackBall* getTrackBall() { return pTrackBall; }
    inline TrackHead* getTrackWalk() { return pTrackHead; }

  protected:
    ViewPoint viewPoint;
    TrackBall* pTrackBall;
    TrackHead* pTrackHead;
};
} // namespace Chimera
#endif
