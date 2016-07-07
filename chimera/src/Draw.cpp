#include "Draw.h"

namespace Chimera {
namespace Graph {

Draw::Draw ( DrawType _type, std::string _name ) : Entity ( EntityKind::DRAW, _name ), type ( _type ) {
    //pMaterial = nullptr;
}

Draw::Draw ( const Draw &_draw ) : Entity ( _draw ) {
    //pMaterial = _draw.pMaterial;
}

Draw::~Draw() {
}

}
}
