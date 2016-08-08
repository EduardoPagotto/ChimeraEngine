#include "HUD.h"
#include "NodeVisitor.h"
#include "ExceptionSDL.h"

namespace Chimera {

HUD::HUD(Node *_parent, std::string _name) : on ( true ), Draw (_parent, EntityKind::HUD, _name) {

#ifdef TTF_NOVO
    if ( !TTF_WasInit() && TTF_Init() == -1 ) {
        throw ExceptionSDL ( ExceptionCode::ALLOC, std::string ( SDL_GetError() ) );
    }
#endif

}

HUD::~HUD() {

    while ( !vFonts.empty() ) {
        Font *pFont = vFonts.back();
        vFonts.pop_back();
        delete pFont;
        pFont = nullptr;
    }

    while ( !vSquare.empty() ) {

        HUDSquare *pScare = vSquare.back();
        vSquare.pop_back();
        delete pScare;
        pScare = nullptr;
    }

    while ( !vLineText.empty() ) {
        HUDTxt *lixo = vLineText.back();
        vLineText.pop_back();
        delete lixo;
    }

#ifdef TTF_NOVO
    TTF_Quit();
#endif

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

void HUD::drawBoxes() {
    for ( HUDSquare* l_square : vSquare ) {

        int cantoA = l_square->tela.y + l_square->tela.h;
        int cantoB = l_square->tela.x + l_square->tela.w;

        glColor4fv ( l_square->color.ptr() );
        glBegin ( GL_QUADS );

        glVertex2i ( l_square->tela.x, cantoA );
        glVertex2i ( cantoB, cantoA );
        glVertex2i ( cantoB, l_square->tela.y );
        glVertex2i ( l_square->tela.x, l_square->tela.y );

        glEnd();

    }
}

void HUD::drawFonts() {

    for ( HUDTxt *l_pTxt : vLineText ) {

        HUDSquare* l_square = vSquare[l_pTxt->indexSquare];

        int l_novoX = l_square->tela.x + l_pTxt->posX;
        int l_novoY = l_square->tela.y + l_pTxt->posY;

        vFonts[l_pTxt->indexFonte]->render ( l_novoX, l_novoY, 0, l_pTxt->color, l_pTxt->pText );
    }
}

void HUD::init() {

	setOn(true);

}

glm::vec3 HUD::getSizeBox()
{
	return glm::vec3();
}

void HUD::renderExecute(bool _texture)
{
	// salva flags de bit
	glPushAttrib(GL_ENABLE_BIT);

	// desabilita oculta face
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// desliga texturas e iluminacao
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// habilita mistura de tons a cena
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//preserva a cor original
	glPushAttrib(GL_CURRENT_BIT);

	//desenha retangulos
	drawBoxes();

	//retorna paleta
	glPopAttrib();

	//habilita testura para as fontes
	glEnable(GL_TEXTURE_2D);//FIXME preciso ??

	//desenha as fontes em texturas
	drawFonts();

	// restaura da pinha de flags
	glPopAttrib();

}

void HUD::accept(NodeVisitor * v) {
	v->visit(this);
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
