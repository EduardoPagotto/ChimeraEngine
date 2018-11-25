#ifndef __CHIMERA_GROUP__HPP
#define __CHIMERA_GROUP__HPP

#include "Coord.hpp"
#include "Node.hpp"
#include "Shader.hpp"
#include "ShadowMapVisitor.hpp"

namespace Chimera {

class Group : public Node {
  public:
    Group(Node* _parent, std::string _name);
    virtual ~Group();
    void accept(class NodeVisitor* v);
    void init();

    void setShader(Shader* _pShader) { pShader = _pShader; }

    Shader* getShader() { return pShader; }

    void setShadowMap(ShadowMapVisitor* _shadoMapVisitor) { this->shadoMapVisitor = _shadoMapVisitor; }

    NodeVisitor* getNodeVisitor() { return shadoMapVisitor; }

  private:
    Shader* pShader;
    ShadowMapVisitor* shadoMapVisitor;
};
} // namespace Chimera
#endif
