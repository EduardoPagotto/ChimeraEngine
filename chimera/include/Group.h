#ifndef __GROUP_H__
#define __GROUP_H__

#include "Node.h"
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

	void setShaderName(const std::string &_name) {
		shadeName = _name;
	}

	std::string getShaderName() const {
		return shadeName;
	}

	void setShadowMap(ShadowMapVisitor *_shadoMapVisitor) {
		this->shadoMapVisitor = _shadoMapVisitor;
	}

	bool executeShadoMap( Coord *_pCoord );

private:
	std::string shadeName;
	ShadowMapVisitor *shadoMapVisitor;
};
}
#endif
