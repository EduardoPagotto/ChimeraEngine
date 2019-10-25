#ifndef __CHIMERA_GROUP__HPP
#define __CHIMERA_GROUP__HPP

#include "Node.hpp"
#include "NodeVisitor.hpp"
#include "chimera/render/Shader.hpp"

namespace Chimera {

class Group : public Node {
  public:
    Group(Node* _parent, std::string _name);
    virtual ~Group();
    void accept(class NodeVisitor* v);

    void init();

    void setShader(Shader* _pShader) { pShader = _pShader; }
    Shader* getShader() { return pShader; }

    void setNodeVisitor(NodeVisitor* _shadoMapVisitor) { this->shadoMapVisitor = _shadoMapVisitor; }
    NodeVisitor* getNodeVisitor() { return shadoMapVisitor; }

  private:
    Shader* pShader;
    NodeVisitor* shadoMapVisitor;
};
} // namespace Chimera
#endif
