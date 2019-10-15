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

    for (std::map<std::string, TexImg*>::iterator iTex = mapTex.begin(); iTex != mapTex.end(); iTex++) {

        TexImg* pTex = iTex->second;
        pTex->init();

        std::string name = iTex->first;
        if (name.compare(SHADE_TEXTURE_DIFFUSE) == 0)
            hasDifuse = true;
        else if (name.compare(SHADE_TEXTURE_EMISSIVE) == 0)
            hasEmissive = true;
        else if (name.compare(SHADE_TEXTURE_SPECULA) == 0)
            hasEspecular = true;
    }

    if (mapTex.size() == 0)
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

void MatData::addTexture(const unsigned& indice, TEX_KIND typeTex, TexImg* _pTex) {
    switch (typeTex) {
        case TEX_KIND::DIFFUSE:
            mapTex[SHADE_TEXTURE_DIFFUSE] = _pTex;
            break;
        case TEX_KIND::EMISSIVE:
            mapTex[SHADE_TEXTURE_EMISSIVE] = _pTex;
            break;
        case TEX_KIND::SPECULAR:
            mapTex[SHADE_TEXTURE_SPECULA] = _pTex;
            break;
        default:
            break;
    }

    _pTex->setIndice(indice);
}

void MatData::apply(Shader* _shader) {

    // aplica todos os materiais passados
    for (ShaderValue* shaderMaterial : listMaterial) {
        shaderMaterial->apply(_shader);
    }

    _shader->setGlUniform1i(SHADE_TEXTURE_SELETOR_TIPO_VALIDO, tipoTexturasDisponiveis);

    if (mapTex.size() > 0) {
        for (std::map<std::string, TexImg*>::iterator iTex = mapTex.begin(); iTex != mapTex.end(); iTex++) {
            std::string name = iTex->first;
            TexImg* pTex = iTex->second;

            // FIXME: precisa disto??? testar multiplas copias e texturas
            unsigned indice = pTex->getIndice();

            pTex->apply(indice, name, _shader);
        }
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

} // namespace Chimera