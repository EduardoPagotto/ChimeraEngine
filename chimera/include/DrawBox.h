#ifndef DRAW_BOX_H_
#define DRAW_BOX_H_

#include "Draw.h"

namespace Chimera {

class DrawBox : public Draw {
public:
    DrawBox();
    DrawBox(const Draw &_draw);
    virtual ~Draw();
        
    void setSizeBox(btVector3 _sizeBox) {
        sizeBox = _sizeBox;
    }
    
    virtual void update ( DataMsg *dataMsg );
    
    virtual void init();
    virtual void renderizar();
    virtual btVector3 getSizeBox(void);
   
    
private:
    btVector3 sizeBox;
    
};    

}

#endif