#include "chimera/node/Camera.hpp"
#include "chimera/node/NodeVisitor.hpp"

//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/quaternion.hpp>

namespace Chimera {

Camera::Camera(Node* _pNode, CameraType _type, std::string _name)
    : type(_type), Node(_pNode, EntityKind::CAMERA, _name) {
    viewPoint.zero();
}

Camera::~Camera() {}

void Camera::accept(NodeVisitor* v) { v->visit(this); }
} // namespace Chimera
