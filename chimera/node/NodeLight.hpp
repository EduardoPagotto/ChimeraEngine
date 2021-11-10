#pragma once
#include "Node.hpp"
#include "chimera/node/VisitorInterface.hpp"
#include "chimera/render/Light.hpp"

namespace Chimera {

class NodeLight : public Node {
  public:
    NodeLight(Node* _parent, std::string _name) : Node(_parent, Kind::LIGHT, _name) {}
    virtual ~NodeLight() {}
    virtual void init(){};
    virtual void accept(class VisitorInterface* v) { v->visit(this); }
    Light data;
};
} // namespace Chimera
