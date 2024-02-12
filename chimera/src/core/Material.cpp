#include "chimera/core/visible/Material.hpp"

namespace Chimera {

Material::Material() : valid(false), tipoTexturasDisponiveis(-1) {}
Material::~Material() {}

void Material::setDefaultEffect() {
    setDiffuse(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
    setEmission(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    // setShine(50.0f);
}

void Material::init() {

    if (valid)
        return;

    valid = true;
    bool hasDifuse = false;
    bool hasEspecular = false;
    bool hasEmissive = false;

    tipoTexturasDisponiveis = 0;
    for (const auto& kv : mapTex) {

        if (kv.first.compare(SHADE_TEXTURE_DIFFUSE) == 0) {
            hasDifuse = true;
        } else if (kv.first.compare(SHADE_TEXTURE_SPECULA) == 0) {
            hasEspecular = true;
        } else if (kv.first.compare(SHADE_TEXTURE_EMISSIVE) == 0) {
            hasEmissive = true;
        }

        // int tex
        // kv.second->init();
    }

    if ((hasDifuse == true) && (hasEspecular == false) && (hasEmissive == false))
        tipoTexturasDisponiveis = 1;
    else if ((hasDifuse == true) && (hasEspecular == true) && (hasEmissive == false))
        tipoTexturasDisponiveis = 2;
    else if ((hasDifuse == true) && (hasEspecular == true) && (hasEmissive == true))
        tipoTexturasDisponiveis = 3;
}

void Material::bindMaterialInformation(MapUniform& uniforms, std::vector<Texture*>& vTex) {
    // copy prop material
    uniforms.insert(listMaterial.begin(), listMaterial.end());

    // seletorr de tipo ???
    uniforms[SHADE_TEXTURE_SELETOR_TIPO_VALIDO] = UValue(tipoTexturasDisponiveis);

    // indice de textura
    int indexTex = 0;
    for (const auto& kv : mapTex) {
        vTex.push_back(kv.second);
        uniforms[kv.first] = UValue(indexTex);
        indexTex++;
    }
}
} // namespace Chimera