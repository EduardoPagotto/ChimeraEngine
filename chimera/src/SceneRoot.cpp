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

void SceneRoot::initScene() {
 
    DataMsg dataMsg ( KindOp::START, this, nullptr, nullptr );
    Node::update ( &dataMsg );
    
}

void SceneRoot::draw(Object *pObjeto) {
    
    Chimera::DataMsg dataMsg ( KindOp::DRAW, this, pObjeto, nullptr );
    Node::update ( &dataMsg );
    
}

}
