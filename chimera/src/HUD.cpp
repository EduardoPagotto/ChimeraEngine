#include "HUD.h"

namespace Chimera {

HUD::HUD ( const Rectangular _rec ) : m_status ( true ), m_tela ( _rec ) {

}

HUD::~HUD() {
    m_vpFont.clear();
    while ( m_vText.size() > 0 ) {
        HUDTxt *lixo = m_vText.back();
        m_vText.pop_back();
        delete lixo;
    }
}

void HUD::beginOrtho ( void ) {
    glMatrixMode ( GL_PROJECTION );
    glPushMatrix( );
    glLoadIdentity( );
    glOrtho ( m_tela.initX,  m_tela.width, m_tela.initY, m_tela.height, -1, 1 );
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
    m_vpFont.push_back ( _pFont );
}

void HUD::addSquare ( const Rectangular _rec, Color _color ) {
    HUDSquare *novoSquare = new HUDSquare;
    novoSquare->tela = _rec;
    novoSquare->color = _color;
    m_vpSquare.push_back ( novoSquare );
}

void HUD::addText ( int _fontIndex, int _squareIndex, int _posX, int _posY, Color _colorText, std::string *_txt ) {
    HUDTxt *newTxt = new HUDTxt;
    newTxt->indexFonte = _fontIndex;
    newTxt->indexSquare = _squareIndex;
    newTxt->posX = _posX;
    newTxt->posY = _posY;
    newTxt->color = _colorText;
    newTxt->pText = _txt;

    m_vText.push_back ( newTxt );
}

void HUD::update() {
    if ( m_status ) {

        beginOrtho();

        for ( unsigned l_contSquare = 0; l_contSquare < m_vpSquare.size(); l_contSquare++ ) {
            HUDSquare* l_square = m_vpSquare[l_contSquare];

            glColor4fv ( l_square->color.ptr() );
            glBegin ( GL_QUADS );
            glVertex2i ( l_square->tela.initX, l_square->tela.initY );
            glVertex2i ( l_square->tela.initX, l_square->tela.height );
            glVertex2i ( l_square->tela.width, l_square->tela.height );
            glVertex2i ( l_square->tela.width, l_square->tela.initY );
            glEnd();
        }

        for ( unsigned l_contador = 0; l_contador < m_vText.size() ; l_contador++ ) {
            HUDTxt *l_pTxt = m_vText[l_contador];
            HUDSquare* l_square = m_vpSquare[l_pTxt->indexSquare];
            int l_novoX = l_square->tela.initX + l_pTxt->posX;
            int l_novoY = l_square->tela.initY + l_pTxt->posY;
            m_vpFont[l_pTxt->indexFonte]->render ( l_novoX, l_novoY, l_pTxt->color, l_pTxt->pText );
        }

        endOrtho();

    }
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
