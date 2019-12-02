#ifndef __CHIMERA_NODE_LIGHT__HPP
#define __CHIMERA_NODE_LIGHT__HPP

#include "Node.hpp"
#include "chimera/core/LightData.hpp"
#include "chimera/node/VisitorInterface.hpp"

namespace Chimera {

class NodeLight : public Node {
  public:
    NodeLight(Node* _parent, std::string _name) : Node(_parent, Kind::LIGHT, _name) {}
    virtual ~NodeLight() {}
    virtual void init(){};
    virtual void accept(class VisitorInterface* v) { v->visit(this); }
    Light light;
};
} // namespace Chimera
#endif
