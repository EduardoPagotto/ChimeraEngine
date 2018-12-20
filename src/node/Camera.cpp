#include "chimera/node/Camera.hpp"
#include "chimera/node/NodeVisitor.hpp"

//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/quaternion.hpp>

namespace Chimera {

Camera::Camera(Node* _pNode, std::string _name) : Node(_pNode, EntityKind::CAMERA, _name) {
    viewPoint.zero();
    pTrackBall = nullptr;
    pTrackWalk = nullptr;
}

Camera::~Camera() {
    if (pTrackBall != nullptr)
        delete pTrackBall;

    if (pTrackWalk != nullptr)
        delete pTrackWalk;
}

void Camera::init() {
    if (pTrackBall != nullptr)
        pTrackBall->init(&viewPoint);

    if (pTrackWalk != nullptr)
        pTrackWalk->init(&viewPoint, WalkType::Air);
}

void Camera::createTrackBall() { pTrackBall = new TrackBall(); }

void Camera::createTrackWalk() { pTrackWalk = new TrackWalk(); }

void Camera::accept(NodeVisitor* v) { v->visit(this); }
} // namespace Chimera
