#include "chimera/render/Material.hpp"

namespace Chimera {

Material::Material() { tipoTexturasDisponiveis = -1; }
Material::~Material() {}

void Material::setDefaultEffect() {
    setDiffuse(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
    setEmission(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    // setShine(50.0f);
}

void Material::init() {

    bool hasDifuse = false;
    bool hasEspecular = false;
    bool hasEmissive = false;

    for (std::list<TextureImg*>::iterator iTex = listTex.begin(); iTex != listTex.end(); iTex++) {

        TextureImg* pTex = *iTex;

        pTex->init();
        switch (pTex->getIndex()) {
            case 0:
                hasDifuse = true;
                break;
            case 3:
                hasEmissive = true;
                break;
            case 2:
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

void Material::addTexture(TextureImg* _pTex) { listTex.push_back(_pTex); }

void Material::apply(Shader* _shader) {

    // aplica todos os materiais passados
    for (ShaderValue* shaderMaterial : listMaterial) {
        shaderMaterial->apply(_shader);
    }

    _shader->setUniform1i(SHADE_TEXTURE_SELETOR_TIPO_VALIDO, tipoTexturasDisponiveis);

    if (listTex.size() > 0) {
        for (std::list<TextureImg*>::iterator iTex = listTex.begin(); iTex != listTex.end(); iTex++) {
            TextureImg* pTex = *iTex;
            pTex->apply(_shader);
        }
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

} // namespace Chimera