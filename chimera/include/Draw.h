#ifndef DRAW_H_
#define DRAW_H_

#include <LinearMath/btVector3.h>
#include <GL/gl.h>

#include "Node.h"

namespace Chimera {

enum class DrawType {
    BOX,
    GRID,
    SPHERE,
    MESH
};

class Draw : public Node {
public:
    Draw ( DrawType _type, std::string _id, std::string _name ) : Node ( EntityKind::DRAW, _id, _name ),  type ( _type ) {}
    Draw ( const Draw &_draw ) : Node ( _draw ),  type ( _draw.type ) {}

    virtual ~Draw() {}

    virtual void update ( DataMsg *dataMsg ) {
        Node::update ( dataMsg );
    }

    virtual void clone ( Node **ppNode ) {
        Node::clone ( ppNode );
    }

    virtual void init() = 0;
    virtual void render() = 0;
    virtual btVector3 getSizeBox() = 0;

    DrawType getType() const {
        return type;
    }

protected:
    DrawType type;

};

}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
