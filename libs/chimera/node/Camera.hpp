#ifndef __CHIMERA_CAMERA__HPP
#define __CHIMERA_CAMERA__HPP

#include "Node.hpp"
#include <glm/glm.hpp>

#include "chimera/core/ViewPoint.hpp"

namespace Chimera {

enum class CameraType { Base, Spherical, Land, Air, Ovr };

class Camera : public Node {
  public:
    Camera(Node* _pNode, CameraType _type, std::string _name);
    virtual ~Camera();

    virtual void init() {}

    // Inherited via Node
    virtual void accept(class NodeVisitor* v) override;

    inline void setType(CameraType _type) { type = _type; }
    inline ViewPoint* getViewPoint() { return &viewPoint; }

  protected:
    CameraType type;
    ViewPoint viewPoint;
};
} // namespace Chimera
#endif
