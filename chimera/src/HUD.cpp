#include "HUD.h"

#ifdef WIN32
#include "windows.h"
#endif

#include <ExceptionSDL.h>

#include <GL/gl.h>
#include <GL/glu.h>

namespace Chimera {

HUD::HUD ( const SDL_Rect &_displayArea ) : on ( true ), displayArea ( _displayArea ) {
  
#ifdef TTF_NOVO
    if( TTF_Init() == -1 )
        throw ExceptionSDL ( ExceptionCode::ALLOC, std::string ( SDL_GetError() ) );
#endif        
    
}

HUD::~HUD() {

    vFonts.clear();
    while ( vLineText.size() > 0 ) {

        HUDTxt *lixo = vLineText.back();
        vLineText.pop_back();
        delete lixo;

    }

}

void HUD::beginOrtho ( void ) {

    glMatrixMode ( GL_PROJECTION );
    glPushMatrix( );
    glLoadIdentity( );
    glOrtho ( displayArea.x,  displayArea.w, displayArea.y, displayArea.h, -1, 1 );
    glMatrixMode ( GL_MODELVIEW );
    glPushMatrix( );
    glLoadIdentity( );

}

void HUD::endOrtho ( void ) {

    glMatrixMode ( GL_PROJECTION );
    glPopMatrix( );
    glMatrixMode ( GL_MODELVIEW );
    glPopMatrix( );

}

void HUD::addFont ( Font *_pFont ) {
    vFonts.push_back ( _pFont );
}

void HUD::addSquare ( const SDL_Rect _rec, Color _color ) {

    HUDSquare *novoSquare = new HUDSquare;
    novoSquare->tela = _rec;
    novoSquare->color = _color;
    vSquare.push_back ( novoSquare );

}

void HUD::addText ( int _fontIndex, int _squareIndex, int _posX, int _posY, Color _colorText, std::string *_txt ) {

    HUDTxt *newTxt = new HUDTxt;
    newTxt->indexFonte = _fontIndex;
    newTxt->indexSquare = _squareIndex;
    newTxt->posX = _posX;
    newTxt->posY = _posY;
    newTxt->color = _colorText;
    newTxt->pText = _txt;

    vLineText.push_back ( newTxt );
}

void HUD::update() {
    if ( on ) {

        glPushAttrib(GL_ENABLE_BIT);
        beginOrtho();

        for (HUDSquare* l_square: vSquare) {
            
            glColor4fv ( l_square->color.ptr() );
            glBegin ( GL_QUADS );
            glVertex2i ( l_square->tela.x, l_square->tela.y );
            glVertex2i ( l_square->tela.x, l_square->tela.h );
            glVertex2i ( l_square->tela.w, l_square->tela.h );
            glVertex2i ( l_square->tela.w, l_square->tela.y );
            glEnd();            
            
        }
        
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        for (HUDTxt *l_pTxt : vLineText) {

            HUDSquare* l_square = vSquare[l_pTxt->indexSquare];
            
            int l_novoX = l_square->tela.x + l_pTxt->posX;
            int l_novoY = l_square->tela.y + l_pTxt->posY;
            
            vFonts[l_pTxt->indexFonte]->render ( l_novoX, l_novoY, 0, l_pTxt->color, l_pTxt->pText);            
        }
        
        endOrtho();

        glPopAttrib();
    }
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
