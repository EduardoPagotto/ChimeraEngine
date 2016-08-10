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
    
    virtual glm::vec3 getRotation() = 0;
    virtual void setRotation(const glm::vec3 &_rot) = 0;
    
    virtual glm::mat4 getMatrix() = 0;
    virtual void setMatrix(const glm::mat4 &_trans) = 0;
    
    virtual void setPositionRotation(const glm::vec3 & _posicao, const glm::vec3 & _rotation) = 0;
    
};
}



#endif