#include "chimera/node/Camera.hpp"
#include "chimera/node/NodeVisitor.hpp"

//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/quaternion.hpp>

namespace Chimera {

Camera::Camera(Node* _pNode, std::string _name) : Node(_pNode, EntityKind::CAMERA, _name) {
    viewPoint.zero();
    pTrackBall = nullptr;
    pTrackHead = nullptr;
}

Camera::~Camera() {
    if (pTrackBall != nullptr)
        delete pTrackBall;

    if (pTrackHead != nullptr)
        delete pTrackHead;
}

void Camera::init() {
    if (pTrackBall != nullptr)
        pTrackBall->init(&viewPoint);

    if (pTrackHead != nullptr)
        pTrackHead->init(&viewPoint);
}

void Camera::createTrackBall() { pTrackBall = new TrackBall(); }

void Camera::createTrackHead() { pTrackHead = new TrackHead(); }

void Camera::accept(NodeVisitor* v) { v->visit(this); }
} // namespace Chimera
