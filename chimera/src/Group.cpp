#include "Group.h"
#include "NodeVisitor.h"

#include "OpenGLDefs.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

Group::Group(Node* _parent, std::string _name) : Node (_parent, EntityKind::GROUP, _name) {
}

Group::~Group() {
}

void Group::accept(NodeVisitor* v){
    v->visit(this);
}

void Group::init() {
}
}


