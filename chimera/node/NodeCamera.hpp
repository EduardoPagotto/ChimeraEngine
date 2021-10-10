#ifndef __CHIMERA_NODE_CAMERA__HPP
#define __CHIMERA_NODE_CAMERA__HPP

#include "Node.hpp"
#include "chimera/core/ICamera.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class NodeCamera : public Node {
  public:
    NodeCamera(Node* _pNode, std::string _name);
    virtual ~NodeCamera();

    virtual void init() override;
    virtual void accept(class VisitorInterface* v) override;
    inline ICamera* getCamera() { return camera; }
    inline void setCamera(ICamera* cam) { this->camera = cam; }

  protected:
    ICamera* camera;
};
} // namespace Chimera
#endif
