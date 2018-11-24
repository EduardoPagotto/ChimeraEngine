#ifndef DRAW_H_
#define DRAW_H_

#include "Node.h"
#include "glm/glm.hpp"
#include "Material.h"

namespace Chimera {

class Draw : public Node
{
public:
    Draw (Node *_parent, EntityKind _kind, std::string _name );
    Draw ( const Draw &_draw );
    virtual ~Draw();

    virtual void render(Shader *_pShader) = 0;
    virtual glm::vec3 getSizeBox() = 0;

	void setMaterial(Material* _pMat) {
		this->material = _pMat;
	}

	Material* getMaterial() const {
		return material;
	}

protected:
	Material *material;
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
