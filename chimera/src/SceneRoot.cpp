#include "SceneRoot.h"
#include "NodeVisitor.h"

namespace Chimera {

SceneRoot::SceneRoot(Node * _pNode, std::string _name) : Node(_pNode, EntityKind::SCENEROOT, _name)
{
}

SceneRoot::~SceneRoot()
{
}

void SceneRoot::accept(NodeVisitor* v)
{
	v->visit(this);
}

}