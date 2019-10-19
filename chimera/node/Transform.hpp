#ifndef __CHIMERA_TRANSFORM__HPP
#define __CHIMERA_TRANSFORM__HPP

#include "Coord.hpp"
#include "chimera/node/NodeVisitor.hpp"
#include "chimera/render/TransData.hpp"

namespace Chimera {

// FIXME: Mudar para operacao matrix
class Transform : public Coord {
  public:
    Transform(Node* _parent, std::string _name) : Coord(_parent, EntityKind::TRANSFORM, _name) {}
    virtual ~Transform() {}

    virtual void accept(class NodeVisitor* v) override { v->visit(this); }

    // Inherited via Coord
    virtual glm::vec3 getPosition() override { return trans.getPosition(); }
    virtual void setPosition(const glm::vec3& _pos) override { trans.setPosition(_pos); }
    virtual glm::vec3 getRotation() override { return trans.getRotation(); }
    virtual void setRotation(const glm::vec3& _rotation) override { trans.setRotation(_rotation); }
    virtual glm::mat4 getMatrix() override { return trans.getMatrix(); }
    virtual glm::mat4 getModelMatrix(Coord* _pCoord) { return trans.getModelMatrix(_pCoord->getPosition()); }
    virtual void setMatrix(const glm::mat4& _trans) override { trans.setMatrix(_trans); }
    inline void setStatic(const bool& _val) { trans.setStatic(_val); }
    inline bool getStatic() const { return trans.getStatic(); }

  private:
    Chimera::TransData trans;
};
} // namespace Chimera
#endif
