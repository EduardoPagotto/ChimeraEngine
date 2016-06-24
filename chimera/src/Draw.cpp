#include "Draw.h"

namespace Chimera {
namespace Graph {

Draw::Draw ( DrawType _type, std::string _id, std::string _name ) : Entity ( EntityKind::DRAW, _id, _name ), type ( _type ) {
    pMaterial = nullptr;
}

Draw::Draw ( const Draw &_draw ) : Entity ( _draw ) {
    pMaterial = _draw.pMaterial;
}

Draw::~Draw() {
}

void Draw::init() {

    //TODO implementar
//     pMaterial = ( Material* ) findChildByKind ( EntityKind::MATERIAL, 0 );
//     if ( pMaterial == nullptr ) {
//         pMaterial = new Material ( getId() + "_Material_defaultCreate", getName() + "_Material_default_create" );
//         pMaterial->createDefaultEffect();
//     }

}

void Draw::render ( bool _texture ) {

    pMaterial->begin ( _texture );
    renderExecute ( _texture );
    pMaterial->end();

}

// void Draw::update ( DataMsg *dataMsg ) {
// 
//     //inicialize primeiro os filhos para garantir textura e efeito em material
//     Node::update ( dataMsg );
// 
//     //inicializa Mesh local
//     if ( dataMsg->getKindOp() == KindOp::START ) {
//         init();
//     }
// 
// }
}
}
