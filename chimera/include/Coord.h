#ifndef __COORD_H__
#define __COORD_H__

#include "Group.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>

namespace Chimera {

class Coord : public Group
{
public:
	Coord(Node* _parent, std::string _name);
	virtual ~Coord();
	void accept(class NodeVisitor* v);

	virtual btVector3 getPosition() = 0;
	virtual void setPosition(const btVector3 &_pos) = 0;

};
}



#endif