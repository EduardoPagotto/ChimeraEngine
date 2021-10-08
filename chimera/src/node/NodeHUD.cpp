#include "chimera/node/NodeHUD.hpp"
#include "chimera/node/VisitorInterface.hpp"

namespace Chimera {

NodeHUD::NodeHUD(Node* _parent, std::string _name) : on(true), Node(_parent, Kind::HUD, _name) {}

NodeHUD::~NodeHUD() {
    // TODO: implementar release
}

void NodeHUD::addFont(Font* _pFont) { vFonts.push_back(_pFont); }

void NodeHUD::addText(int _fontIndex, const glm::ivec2& _pos, glm::vec4 _colorText, GLfloat _scale, std::string* _txt) {

    HUDTxt* newTxt = new HUDTxt;
    newTxt->indexFonte = _fontIndex;
    newTxt->pos = _pos;
    newTxt->color = _colorText;
    newTxt->scale = _scale;
    newTxt->pText = _txt;

    vLineText.push_back(newTxt);
}

void NodeHUD::init() { setOn(true); }

void NodeHUD::render(Shader* _pShader) {
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
        _pShader->setUniform("textColor", l_pTxt->color);
        vFonts[l_pTxt->indexFonte]->renderText(l_pTxt->pText, l_pTxt->pos, l_pTxt->scale);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopAttrib();
    glPopAttrib();
}

void NodeHUD::accept(VisitorInterface* v) { v->visit(this); }
} // namespace Chimera