#ifndef __GROUP_H__
#define __GROUP_H__

#include "Node.h"
#include "OpenGLDefs.h"

#include <glm/glm.hpp>



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

private:
	std::string shadeName;
};
}
#endif
