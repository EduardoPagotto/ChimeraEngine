#include "HUD.h"
#include "NodeVisitor.h"
#include "ExceptionSDL.h"

namespace Chimera {

HUD::HUD(Node *_parent, std::string _name) : on ( true ), Draw (_parent, EntityKind::HUD, _name) {
}

HUD::~HUD() {
	//TODO: implementar release
}

void HUD::addFont ( Font *_pFont ) {
    vFonts.push_back ( _pFont );
}

void HUD::addText ( int _fontIndex, int _posX, int _posY, Color _colorText, GLfloat _scale, std::string *_txt ) {

    HUDTxt *newTxt = new HUDTxt;
    newTxt->indexFonte = _fontIndex;
    newTxt->posX = _posX;
    newTxt->posY = _posY;
    newTxt->color = _colorText;
    newTxt->scale = _scale;
    newTxt->pText = _txt;

    vLineText.push_back ( newTxt );
}

void HUD::drawFonts() {

	// Activate corresponding render state
	shade.link();

	glActiveTexture(GL_TEXTURE0);

	for (HUDTxt *l_pTxt : vLineText) {

		shade.setGlUniform4fv("textColor", 1, l_pTxt->color.ptr());

		vFonts[l_pTxt->indexFonte]->RenderText( l_pTxt->pText, l_pTxt->posX, l_pTxt->posY, l_pTxt->scale);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void HUD::init() { 

	setOn(true);
}

glm::vec3 HUD::getSizeBox() { 

	return glm::vec3();
}

void HUD::renderExecute(bool _texture)
{
    // salva flags de bit
	glPushAttrib(GL_ENABLE_BIT);
    
    //preserva a cor original
	glPushAttrib(GL_CURRENT_BIT);
    
    // Set OpenGL options
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Compile and setup the shader
    //Shader shader("shaders/text.vs", "shaders/text.frag");
    //glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
    //shader.Use();
    //glUniformMatrix4fv(glGetUniformLocation(shade.getIdProgram() , "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    drawFonts();
    
    glPopAttrib();
    glPopAttrib();
}

void HUD::accept(NodeVisitor * v) {
	v->visit(this);
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
