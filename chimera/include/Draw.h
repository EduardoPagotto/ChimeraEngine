#ifndef DRAW_H_
#define DRAW_H_

#include "Node.h"
#include "State.h"
#include "glm/glm.hpp"

namespace Chimera {

class Draw : public Node
{
public:
    friend class LoaderDae;
    Draw (Node *_parent, EntityKind _kind, std::string _name );
    Draw ( const Draw &_draw );
    virtual ~Draw();
    virtual void renderExecute ( bool _texture ) = 0;
    virtual glm::vec3 getSizeBox() = 0;

    State *getState() {
        return pState;
    }
    
    void setTexture(Texture *_pTex) {
		pState->setEnableTexture(_pTex, true);
    }
    
    void setMaterial(Material *_pMat) {
		pState->setEnableMaterial(_pMat, true);
    }
    
protected:

    State *pState;    
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
