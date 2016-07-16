#ifndef DRAW_H_
#define DRAW_H_

#include <LinearMath/btVector3.h>

#include "Node.h"
#include "State.h"

namespace Chimera {

enum class DrawType
{
    MESH,
    PARTICLE_SYSTEM,
	HUD
};

class Draw : public Node
{
public:
    Draw (Node *_parent, DrawType _type, std::string _name );
    Draw ( const Draw &_draw );
    virtual ~Draw();
	virtual void init();
	virtual void accept(class NodeVisitor* v);
    virtual void renderExecute ( bool _texture ) = 0;
    virtual btVector3 getSizeBox() = 0;

    DrawType getType() const {
        return type;
    }

    State *getState() {
        return pState;
    }
    
    Texture *pTexture;
    Material *pMaterial;
    
protected:
    DrawType type;
    State *pState;    
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
