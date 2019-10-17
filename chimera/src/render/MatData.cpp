#include "chimera/render/MatData.hpp"

namespace Chimera {

MatData::MatData() { tipoTexturasDisponiveis = -1; }
MatData::~MatData() {}

void MatData::setDefaultEffect() {
    setDiffuse(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
    setEmission(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    // setShine(50.0f);
}

void MatData::init() {

    bool hasDifuse = false;
    bool hasEspecular = false;
    bool hasEmissive = false;

    for (std::list<TexImg*>::iterator iTex = listTex.begin(); iTex != listTex.end(); iTex++) {

        TexImg* pTex = *iTex;

        pTex->init();
        switch (pTex->getKind()) {
            case TEX_KIND::DIFFUSE:
                hasDifuse = true;
                break;
            case TEX_KIND::EMISSIVE:
                hasEmissive = true;
                break;
            case TEX_KIND::SPECULAR:
                hasEspecular = true;
                break;
            default:
                break;
        }
    }

    if (listTex.size() == 0)
        tipoTexturasDisponiveis = 0;
    else if ((hasDifuse == true) && (hasEspecular == false) && (hasEmissive == false))
        tipoTexturasDisponiveis = 1;
    else if ((hasDifuse == true) && (hasEspecular == true) && (hasEmissive == false))
        tipoTexturasDisponiveis = 2;
    else if ((hasDifuse == true) && (hasEspecular == true) && (hasEmissive == true))
        tipoTexturasDisponiveis = 3;
    else
        tipoTexturasDisponiveis = 4;
}

void MatData::addTexture(TexImg* _pTex) { listTex.push_back(_pTex); }

void MatData::apply(Shader* _shader) {

    // aplica todos os materiais passados
    for (ShaderValue* shaderMaterial : listMaterial) {
        shaderMaterial->apply(_shader);
    }

    _shader->setGlUniform1i(SHADE_TEXTURE_SELETOR_TIPO_VALIDO, tipoTexturasDisponiveis);

    if (listTex.size() > 0) {
        for (std::list<TexImg*>::iterator iTex = listTex.begin(); iTex != listTex.end(); iTex++) {
            TexImg* pTex = *iTex;
            pTex->apply(_shader);
        }
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

} // namespace Chimera