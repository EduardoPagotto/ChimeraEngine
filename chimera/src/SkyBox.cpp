#include "SkyBox.h"
#include "NodeVisitor.h"
//#include <Object.h>

#ifdef WIN32
#include "windows.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {
   
SkyBox::SkyBox ( Node *_parent, std::string _name ) : Node (_parent, EntityKind::SKYBOX, _name ) {

}

SkyBox::~SkyBox() {

}

void SkyBox::init() {

	pMaterial->init();
	pTexture->init();

	Node::init();
}

void SkyBox::update ( DataMsg *_dataMsg ) {

    Node::update ( _dataMsg );
}

void SkyBox::accept(NodeVisitor* v)
{
	v->visit(this);
}

void SkyBox::render ( bool _texture ) {

   if ( pDraw != nullptr ) {

       glPushAttrib ( GL_ENABLE_BIT );
       glPushAttrib ( GL_CURRENT_BIT );

       // Enable smooth shading
       glShadeModel ( GL_SMOOTH );
       glDisable ( GL_LIGHTING );
            
        if (( _texture == true ) && (pTexture != nullptr))
            pTexture->begin();
        
        pMaterial->apply();
        
        //pMaterial->begin(_texture);
        pDraw->renderExecute(_texture);
    
    if (( _texture == true ) && (pTexture != nullptr))
        pTexture->end();
           
       glPopAttrib();
       glPopAttrib();
   }
}
}