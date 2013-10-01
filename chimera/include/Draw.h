#ifndef DRAW_H_
#define DRAW_H_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Material.h"
#include "Transform.h"

namespace Chimera {

enum class DRAW_TYPE {
	DRAW_NONE,
	BOX,
	BOXGRID,
	BOXGRID2,
	SPHERE,
	TRI_MESH
};

class Draw {
public:
	Draw();
	virtual ~Draw();
    virtual void renderizar();
	virtual btVector3 sizeQuadratic(void) = 0;
	
	void init();
	void ajusteMatrix(Transform *_pTrans);

protected:
	
	Material *m_pMaterial;
	DRAW_TYPE m_typeDraw;
};

}
#endif