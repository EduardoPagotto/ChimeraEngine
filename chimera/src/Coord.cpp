#include "Coord.h"
#include "NodeVisitor.h"

namespace Chimera {

Coord::Coord(Node * _parent, std::string _name) : Group (_parent, _name){
}

Coord::~Coord(){
}

void Coord::accept(NodeVisitor * v){
	v->visit(this);
}

}