#include "chimera_core/gl/Material.hpp"

namespace ce {

    Material::Material() : valid(false), tipoTexturasDisponiveis(-1) {}
    Material::~Material() {}

    void Material::setDefaultEffect() {
        setDiffuse(glm::vec4(0.6F, 0.6F, 0.6F, 1.0F));
        setEmission(glm::vec4(0.1F, 0.1F, 0.1F, 1.0F));
        setAmbient(glm::vec4(0.1F, 0.1F, 0.1F, 1.0F));
        setSpecular(glm::vec4(0.5F, 0.5F, 0.5F, 1.0F));
        // setShine(50.0f);
    }

    void Material::init() {

        if (valid) {
            return;
        }

        valid = true;
        bool hasDifuse = false;
        bool hasEspecular = false;
        bool hasEmissive = false;

        tipoTexturasDisponiveis = 0;
        for (const auto& kv : mapTex) {

            if (kv.first == SHADE_TEXTURE_DIFFUSE) {
                hasDifuse = true;
            } else if (kv.first == SHADE_TEXTURE_SPECULA) {
                hasEspecular = true;
            } else if (kv.first == SHADE_TEXTURE_EMISSIVE) {
                hasEmissive = true;
            }

            // int tex
            // kv.second->init();
        }

        if ((hasDifuse) && (!hasEspecular) && (!hasEmissive)) {
            tipoTexturasDisponiveis = 1;
        } else if ((hasDifuse) && (hasEspecular) && (!hasEmissive)) {
            tipoTexturasDisponiveis = 2;
        } else if ((hasDifuse) && (hasEspecular) && (hasEmissive)) {
            tipoTexturasDisponiveis = 3;
        }
    }

    void Material::bindMaterialInformation(MapUniform& uniforms, std::vector<std::shared_ptr<Texture>>& vTex) {
        // copy prop material
        uniforms.insert(listMaterial.begin(), listMaterial.end());

        // FIXME: seletorr de tipo ???
        uniforms[SHADE_TEXTURE_SELETOR_TIPO_VALIDO] = Uniform(tipoTexturasDisponiveis);

        // indice de textura
        int indexTex = 0;
        for (const auto& kv : mapTex) {
            vTex.push_back(kv.second);
            uniforms[kv.first] = Uniform(indexTex);
            indexTex++;
        }
    }
} // namespace ce
