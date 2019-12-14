#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/VisitorInterface.hpp"

namespace Chimera {

NodeCamera::NodeCamera(Node* _pNode, std::string _name) : Node(_pNode, Kind::CAMERA, _name) {
    viewPoint.zero();
    pTrackBall = nullptr;
    pTrackHead = nullptr;
}

NodeCamera::~NodeCamera() {
    if (pTrackBall != nullptr)
        delete pTrackBall;

    if (pTrackHead != nullptr)
        delete pTrackHead;
}

void NodeCamera::init() {
    if (pTrackBall != nullptr)
        pTrackBall->init(&viewPoint);

    if (pTrackHead != nullptr)
        pTrackHead->init(&viewPoint);
}

void NodeCamera::createTrackBall() { pTrackBall = new TrackBall(); }

void NodeCamera::createTrackHead() { pTrackHead = new TrackHead(); }

void NodeCamera::accept(VisitorInterface* v) { v->visit(this); }
} // namespace Chimera
