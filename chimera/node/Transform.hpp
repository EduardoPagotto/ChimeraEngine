#ifndef __CHIMERA_TRANSFORM__HPP
#define __CHIMERA_TRANSFORM__HPP

#include "ITransform.hpp"
#include "chimera/render/TransData.hpp"

namespace Chimera {

// FIXME: Mudar para operacao matrix
class Transform : public ITransform {
  public:
    Transform() : ITransform() {}
    virtual ~Transform() noexcept {}

    // Inherited via Coord
    virtual glm::vec3 getPosition() override { return trans.getPosition(); }
    virtual void setPosition(const glm::vec3& _pos) override { trans.setPosition(_pos); }

    virtual glm::vec3 getRotation() override { return trans.getRotation(); }
    virtual void setRotation(const glm::vec3& _rotation) override { trans.setRotation(_rotation); }

    virtual glm::mat4 getMatrix() override { return trans.getMatrix(); }
    virtual void setMatrix(const glm::mat4& _trans) override { trans.setMatrix(_trans); }

    virtual glm::mat4 getModelMatrix(const glm::vec3& _pos) override { return trans.getModelMatrix(_pos); }
    virtual void init(const glm::vec3& _size) override {}

  private:
    Chimera::TransData trans;
};
} // namespace Chimera
#endif
