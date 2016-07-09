#include "Draw.h"
#include "NodeVisitor.h"

namespace Chimera {

Draw::Draw (Node *_parent, DrawType _type, std::string _name ) : Node (_parent, EntityKind::DRAW, _name ), type ( _type ) {
   
}

Draw::Draw ( const Draw &_draw ) : Node( _draw ) {
    
}

Draw::~Draw() {
}

void Draw::accept(NodeVisitor * v)
{
	v->visit(this);
}

}
