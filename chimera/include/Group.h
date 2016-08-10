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
    
    void setIdProgram(const GLuint &_id);
    
    GLuint getIdProgram() const {
        return idProgram;
    }
    
    GLuint applyIdProgram(const glm::mat4 &_view,const glm::mat4 &_proj);
    
private:
	 GLuint idProgram;
};
}
#endif
