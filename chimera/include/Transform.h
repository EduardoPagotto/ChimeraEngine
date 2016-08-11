#ifndef __TRANSFORM_H__
#define	__TRANSFORM_H__

#include "Coord.h"

namespace Chimera {

//FIXME: Mudar para operacao matrix
class Transform : public Coord {
public:
	Transform(Node* _parent, std::string _name);
	virtual ~Transform();

	void accept(class NodeVisitor * v);

	// Inherited via Coord
    virtual glm::vec3 getPosition() override;
    virtual void setPosition(const glm::vec3 &_pos) override;
    virtual glm::vec3 getRotation() override;
    virtual void setRotation ( const glm::vec3 &_rotation ) override;
    virtual glm::mat4 getMatrix() override;
    virtual void setMatrix(const glm::mat4 &_trans) override;
    virtual void setPositionRotation(const glm::vec3 & _posicao, const glm::vec3 & _rotation) override;
    
private:

    glm::mat4 model;
};
}
#endif	/* TRANSFORM_H */
