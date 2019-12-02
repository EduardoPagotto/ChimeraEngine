#ifndef __CHIMERA_GROUP__HPP
#define __CHIMERA_GROUP__HPP

#include "Node.hpp"
#include "VisitorInterface.hpp"
#include "chimera/core/Shader.hpp"

namespace Chimera {

class NodeGroup : public Node {
  public:
    NodeGroup(Node* _parent, std::string _name);
    virtual ~NodeGroup();

    virtual void init() override;
    virtual void accept(class VisitorInterface* v) override;

    void setShader(Shader* _pShader) { pShader = _pShader; }
    Shader* getShader() { return pShader; }

    void setNodeVisitor(VisitorInterface* _shadoMapVisitor) { this->shadoMapVisitor = _shadoMapVisitor; }
    VisitorInterface* getNodeVisitor() { return shadoMapVisitor; }

  private:
    Shader* pShader;
    VisitorInterface* shadoMapVisitor;
};
} // namespace Chimera
#endif
