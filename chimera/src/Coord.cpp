#include "Coord.h"
#include "NodeVisitor.h"

namespace Chimera {

Coord::Coord(Node* _parent, EntityKind _entity, std::string _name) : Node (_parent, _entity , _name){
}

Coord::~Coord(){
}

}