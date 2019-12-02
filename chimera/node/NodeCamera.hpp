#ifndef __CHIMERA_CAMERA__HPP
#define __CHIMERA_CAMERA__HPP

#include "Node.hpp"
#include "chimera/core/TrackBall.hpp"
#include "chimera/core/TrackHead.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class NodeCamera : public Node {
  public:
    NodeCamera(Node* _pNode, std::string _name);
    virtual ~NodeCamera();

    virtual void init() override;
    virtual void accept(class NodeVisitor* v) override;
    void createTrackBall();
    void createTrackHead();

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
