#ifndef __CHIMERA_CAMERA__HPP
#define __CHIMERA_CAMERA__HPP

#include "Node.hpp"
#include <glm/glm.hpp>

//#include "chimera/core/ViewPoint.hpp"

#include "chimera/core/TrackBall.hpp"
#include "chimera/core/TrackWalk.hpp"

namespace Chimera {

class Camera : public Node {
  public:
    Camera(Node* _pNode, std::string _name);
    virtual ~Camera();

    virtual void init();
    void createTrackBall();
    void createTrackWalk();

    // Inherited via Node
    virtual void accept(class NodeVisitor* v) override;

    inline ViewPoint* getViewPoint() { return &viewPoint; }
    inline TrackBall* getTrackBall() { return pTrackBall; }
    inline TrackWalk* getTrackWalk() { return pTrackWalk; }

  protected:
    ViewPoint viewPoint;
    TrackBall* pTrackBall;
    TrackWalk* pTrackWalk;
};
} // namespace Chimera
#endif
