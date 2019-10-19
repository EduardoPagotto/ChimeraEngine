#include "chimera/node/HUD.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/node/NodeVisitor.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

HUD::HUD(Node* _parent, std::string _name) : on(true), Draw(_parent, EntityKind::HUD, _name) {}

HUD::~HUD() {
    // TODO: implementar release
}

void HUD::addFont(Font* _pFont) { vFonts.push_back(_pFont); }

void HUD::addText(int _fontIndex, int _posX, int _posY, glm::vec4 _colorText, GLfloat _scale, std::string* _txt) {

    HUDTxt* newTxt = new HUDTxt;
    newTxt->indexFonte = _fontIndex;
    newTxt->posX = _posX;
    newTxt->posY = _posY;
    newTxt->color = _colorText;
    newTxt->scale = _scale;
    newTxt->pText = _txt;

    vLineText.push_back(newTxt);
}

void HUD::init() { setOn(true); }

glm::vec3 HUD::getSizeBox() { return glm::vec3(); }

void HUD::render(Shader* _pShader) {
    // salva flags de bit
    glPushAttrib(GL_ENABLE_BIT);

    // preserva a cor original
    glPushAttrib(GL_CURRENT_BIT);

    // Set OpenGL options
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);

    for (HUDTxt* l_pTxt : vLineText) {

        _pShader->setGlUniform4fv("textColor", 1, glm::value_ptr(l_pTxt->color));

        vFonts[l_pTxt->indexFonte]->RenderText(l_pTxt->pText, l_pTxt->posX, l_pTxt->posY, l_pTxt->scale);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopAttrib();
    glPopAttrib();
}

void HUD::accept(NodeVisitor* v) { v->visit(this); }
} // namespace Chimera