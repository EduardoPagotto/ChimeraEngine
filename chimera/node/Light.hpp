#ifndef __CHIMERA_LIGHT__HPP
#define __CHIMERA_LIGHT__HPP

#include "Node.hpp"
#include "chimera/core/LightData.hpp"

namespace Chimera {

class Light : public Node {
  public:
    Light(Node* _parent, std::string _name);
    virtual ~Light();
    virtual void init() override;
    virtual void accept(class NodeVisitor* v) override;

    LightData lightData;
};
} // namespace Chimera
#endif
