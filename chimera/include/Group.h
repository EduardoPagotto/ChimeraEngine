#ifndef __GROUP_H__
#define __GROUP_H__

#include "Node.h"
#include "OpenGLDefs.h"

#include <glm/glm.hpp>

#include <Shader.h>

namespace Chimera {

class Group : public Node
{
public:
    Group(Node* _parent, std::string _name);
    virtual ~Group();
    void accept(class NodeVisitor* v);
    void apply(const glm::mat4 &_view,const glm::mat4 &_proj);
	void init();

	void setShaderName(const std::string &_name) {
		shadeName = _name;
	}

private:
	std::string shadeName;
    Shader *shader;
};
}
#endif
