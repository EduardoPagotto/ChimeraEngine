#include "SkyBox.h"
//#include <Object.h>

#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {
namespace Graph {
   
SkyBox::SkyBox ( std::string _id, std::string _name ) : Node ( EntityKind::SKYBOX, _id, _name ) {

}

SkyBox::~SkyBox() {

}

void SkyBox::update ( DataMsg *_dataMsg ) {

     if ( _dataMsg->getKindOp() == KindOp::START ) {
		 pMaterial->init();
// 
//         //inicialize primeiro os filhos para garantir textura e efeito em material
//         Node::update ( _dataMsg );
// 
//         //inicializa objeto local
//         //pDraw = ( Draw* ) findChildByKind ( EntityKind::DRAW, 0 );
// 
     } else {
// 
//         Node::update ( _dataMsg );
// 
     }
    Node::update ( _dataMsg );

}

void SkyBox::render ( bool _texture ) {

   if ( pDraw != nullptr ) {

       glPushAttrib ( GL_ENABLE_BIT );
       glPushAttrib ( GL_CURRENT_BIT );

       // Enable smooth shading
       glShadeModel ( GL_SMOOTH );
       glDisable ( GL_LIGHTING );

       pMaterial->begin(_texture);
       
       pDraw->renderExecute ( _texture );

       pMaterial->end();
       
       glPopAttrib();
       glPopAttrib();

   }

}
}
}