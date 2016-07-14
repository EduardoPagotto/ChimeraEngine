#include "Draw.h"
#include "NodeVisitor.h"

namespace Chimera {

Draw::Draw (Node *_parent, DrawType _type, std::string _name ) : Node (_parent, EntityKind::DRAW, _name ), type ( _type ) {
    
    pState = new State();
}

Draw::Draw ( const Draw &_draw ) : Node( _draw ) { 
}

Draw::~Draw() {
}

void Draw::init() {
	Node::init();
}

void Draw::accept(NodeVisitor * v) {
	v->visit(this);
}

}
