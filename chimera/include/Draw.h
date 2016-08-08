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
        pTexture = _pTex;
    }
    
    void setMaterial(Material *_pMat) {
        pMaterial = _pMat;
    }
    
protected:
    Texture *pTexture;
    Material *pMaterial;
    State *pState;    
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
