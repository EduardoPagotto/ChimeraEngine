#ifndef __TRANSFORM_H__
#define	__TRANSFORM_H__

#include "Coord.h"

namespace Chimera {

//FIXME: Mudar para operacao matrix
class Transform : public Coord {
public:

	Transform(Node* _parent, std::string _name);
	virtual ~Transform();

	// Inherited via Group
	virtual btVector3 getPosition() override;
	virtual void setPosition(const btVector3 & _pos) override;
	

	void accept(class NodeVisitor* v);

	void setTranslate(const btVector3 &_vet);
	btVector3 getTranslate();
	bool getTranslateFlag();

	void setRotate(const btVector3 &_vet);
	btVector3 getRotate();
	bool getRotateFlag();

	void setScale(const btVector3 &_vet);
	btVector3 getScale();

	bool getScaleFlag();
	void applyTransform();

    void setTransform( const btTransform &_trans );
    
private:
	bool tflag;
	bool rflag;
	bool sflag;

	btVector3 translate;
	btVector3 rotate; 
	btVector3 scale; 


};
}

#endif	/* TRANSFORM_H */

