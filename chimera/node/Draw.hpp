#ifndef __CHIMERA_DRAW__HPP
#define __CHIMERA_DRAW__HPP

#include "Node.hpp"
#include "chimera/render/MatData.hpp"
#include "glm/glm.hpp"

namespace Chimera {

class Draw : public Node {
  public:
    Draw(Node* _parent, EntityKind _kind, std::string _name);
    Draw(const Draw& _draw);
    virtual ~Draw();

    virtual void render(Shader* _pShader) = 0;
    virtual glm::vec3 getSizeBox() = 0;

    void setMaterial(MatData* _pMat) { this->material = _pMat; }

    MatData* getMaterial() const { return material; }

  protected:
    MatData* material;
};
} // namespace Chimera
#endif
