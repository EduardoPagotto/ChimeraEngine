#ifndef DRAW_H_
#define DRAW_H_

#include <LinearMath/btVector3.h>
#include <GL/gl.h>

namespace Chimera {

enum class DrawType {
	BOX,
	GRID,
	SPHERE,
	MESH
};

class Draw {
public:
    
    friend class Loader;
    
    Draw(DrawType _type) : type(_type) {}
    
    Draw(const Draw &_draw) : type(_draw.type) {}
    
	virtual ~Draw() {}
    
    DrawType getType() const {
        return type;
    }
       
    virtual void init() = 0;
    virtual void render() = 0;
	virtual btVector3 getSizeBox() = 0;

protected:
    DrawType type;
    
};

}
#endif