#ifndef __GROUP_H__
#define __GROUP_H__

#include "Node.h"
#include "OpenGLDefs.h"

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
    
    GLuint applyIdProgram();
    
    
private:
	 GLuint idProgram;
};
}
#endif
