#include "Draw.h"

namespace Chimera {
namespace Graph {

Draw::Draw ( DrawType _type, std::string _id, std::string _name ) : Entity ( EntityKind::DRAW, _id, _name ), type ( _type ) {
    //pMaterial = nullptr;
}

Draw::Draw ( const Draw &_draw ) : Entity ( _draw ) {
    //pMaterial = _draw.pMaterial;
}

Draw::~Draw() {
}

}
}
