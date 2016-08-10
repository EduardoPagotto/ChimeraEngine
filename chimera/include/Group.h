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
    
    //bool createShade(const char* vertex_file_path, const char* fragment_file_path);
    
    Shader shader;
};
}
#endif
