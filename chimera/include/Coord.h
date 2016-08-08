#ifndef __COORD_H__
#define __COORD_H__

#include "Node.h"
#include "glm/glm.hpp"

namespace Chimera {

class Coord : public Node
{
public:
	Coord(Node* _parent, EntityKind _entity, std::string _name) : Node (_parent, _entity , _name){}
	virtual ~Coord(){}
	virtual glm::vec3 getPosition() = 0;
	virtual void setPosition(const glm::vec3 &_pos) = 0;
};
}



#endif