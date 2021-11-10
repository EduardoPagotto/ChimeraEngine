#pragma once
#include "Node.hpp"
#include "VisitorInterface.hpp"
#include "chimera/render/Shader.hpp"

namespace Chimera {

class NodeGroup : public Node {
  public:
    NodeGroup(Node* _parent, std::string _name) : Node(_parent, Kind::GROUP, _name), pShader(nullptr), shadoMapVisitor(nullptr) {}
    virtual ~NodeGroup() {
        if (pShader != nullptr)
            delete pShader;
    }

    virtual void init() override {
        if (shadoMapVisitor != nullptr)
            shadoMapVisitor->init();
    }
    virtual void accept(VisitorInterface* v) override { v->visit(this); }

    void setShader(Shader* _pShader) { pShader = _pShader; }
    Shader* getShader() { return pShader; }

    void setNodeVisitor(VisitorInterface* _shadoMapVisitor) { this->shadoMapVisitor = _shadoMapVisitor; }
    VisitorInterface* getNodeVisitor() { return shadoMapVisitor; }

  private:
    Shader* pShader;
    VisitorInterface* shadoMapVisitor;
};
} // namespace Chimera
