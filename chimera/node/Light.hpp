#ifndef __CHIMERA_LIGHT__HPP
#define __CHIMERA_LIGHT__HPP

#include "Node.hpp"
#include "chimera/core/LightData.hpp"
#include "chimera/node/NodeVisitor.hpp"

namespace Chimera {

class Light : public Node {
  public:
    Light(Node* _parent, std::string _name) : Node(_parent, Kind::LIGHT, _name) {}
    virtual ~Light() {}
    virtual void init() override{};
    virtual void accept(class NodeVisitor* v) override { v->visit(this); };
    LightData lightData;
};
} // namespace Chimera
#endif
