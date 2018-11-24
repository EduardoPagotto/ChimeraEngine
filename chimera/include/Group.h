#ifndef __GROUP_H__
#define __GROUP_H__

#include "Node.h"
#include "Shader.h"
#include "ShadowMapVisitor.h"
#include "Coord.h"

namespace Chimera {

class Group : public Node
{
public:
    Group(Node* _parent, std::string _name);
    virtual ~Group();
    void accept(class NodeVisitor* v);
	void init();

	void setShader(Shader *_pShader) {
		pShader = _pShader;
	}

	Shader *getShader() {
		return pShader;
	}

	void setShadowMap(ShadowMapVisitor *_shadoMapVisitor) {
		this->shadoMapVisitor = _shadoMapVisitor;
	}

	ShadowMap* executeShadoMap( Coord *_pCoord );

private:
	Shader *pShader;
	ShadowMapVisitor *shadoMapVisitor;
};
}
#endif
