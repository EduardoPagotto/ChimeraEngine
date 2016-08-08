#ifndef __TRANSFORM_H__
#define	__TRANSFORM_H__

#include "Coord.h"

namespace Chimera {

//FIXME: Mudar para operacao matrix
class Transform : public Coord {
public:
	Transform(Node* _parent, std::string _name);
	virtual ~Transform();

	// Inherited via Coord
	virtual glm::vec3 getPosition() override;
	virtual void setPosition(const glm::vec3 & _pos) override;
	
    // Inherited via Node
	void accept(class NodeVisitor* v) override;

	//void setTranslate(const btVector3 &_vet);
	//btVector3 getTranslate();
	bool getTranslateFlag();

	void setRotate(const glm::vec3 &_vet);
	glm::vec3 getRotate();
	bool getRotateFlag();

	void setScale(const glm::vec3 &_vet);
	glm::vec3 getScale();

	bool getScaleFlag();
	void applyTransform();

    void setTransform( const glm::mat4 &_trans );
    
private:
	bool tflag;
	bool rflag;
	bool sflag;

	glm::vec3 translate;
	glm::vec3 rotate; 
	glm::vec3 scale; 
};
}
#endif	/* TRANSFORM_H */
