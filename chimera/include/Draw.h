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

    virtual void render() = 0;
    virtual glm::vec3 getSizeBox() = 0;

    inline State *getState() {
        return pState;
    }
        
protected:
    State *pState;    
};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
