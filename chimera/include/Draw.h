#ifndef DRAW_H_
#define DRAW_H_

#include <LinearMath/btVector3.h>

#include "Node.h"
#include "State.h"

namespace Chimera {

class Draw : public Node
{
public:
    Draw (Node *_parent, EntityKind _kind, std::string _name );
    Draw ( const Draw &_draw );
    virtual ~Draw();
    virtual void renderExecute ( bool _texture ) = 0;
    virtual btVector3 getSizeBox() = 0;

    State *getState() {
        return pState;
    }
    
    Texture *pTexture;
    Material *pMaterial;
protected:
    
    State *pState;    
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
