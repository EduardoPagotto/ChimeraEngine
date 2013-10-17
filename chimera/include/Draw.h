#ifndef DRAW_H_
#define DRAW_H_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Material.h"

namespace Chimera {

enum class DrawType {
	BOX,
	GRID,
	BOXGRID2,
	SPHERE,
	MESH
};

class Draw : public Node {
public:
    Draw(DrawType _type);
    Draw(const Draw &_draw);
	virtual ~Draw();
    
    virtual void update ( DataMsg *dataMsg );
    
    virtual void init();
    virtual void renderizar();
	virtual btVector3 getSizeBox() = 0;

	//void ajusteMatrix(Transform *_pTrans);

protected:
	
	Material *pMaterial;
    DrawType type;
};

}
#endif