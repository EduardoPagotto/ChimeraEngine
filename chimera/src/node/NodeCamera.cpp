#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/VisitorInterface.hpp"

namespace Chimera {

NodeCamera::NodeCamera(Node* _pNode, std::string _name) : Node(_pNode, Kind::CAMERA, _name) { camera = nullptr; }

NodeCamera::~NodeCamera() { delete camera; }

void NodeCamera::init() {}

void NodeCamera::accept(VisitorInterface* v) { v->visit(this); }
} // namespace Chimera
