#include "chimera_core/gl/Material.hpp"

namespace ce {

    Material::Material() : valid_(false), tipo_texturas_disponiveis_(-1) {}
    Material::~Material() {}

    void Material::set_default_effect() {
        set_diffuse(glm::vec4(0.6F, 0.6F, 0.6F, 1.0F));
        set_emission(glm::vec4(0.1F, 0.1F, 0.1F, 1.0F));
        set_ambient(glm::vec4(0.1F, 0.1F, 0.1F, 1.0F));
        set_specular(glm::vec4(0.5F, 0.5F, 0.5F, 1.0F));
        // setShine(50.0f);
    }

    void Material::init() {

        if (valid_) {
            return;
        }

        valid_ = true;
        bool has_difuse = false;
        bool has_especular = false;
        bool has_emissive = false;

        tipo_texturas_disponiveis_ = 0;
        for (const auto& kv : map_tex_) {

            if (kv.first == SHADE_TEXTURE_DIFFUSE) {
                has_difuse = true;
            } else if (kv.first == SHADE_TEXTURE_SPECULA) {
                has_especular = true;
            } else if (kv.first == SHADE_TEXTURE_EMISSIVE) {
                has_emissive = true;
            }

            // int tex
            // kv.second->init();
        }

        if ((has_difuse) && (!has_especular) && (!has_emissive)) {
            tipo_texturas_disponiveis_ = 1;
        } else if ((has_difuse) && (has_especular) && (!has_emissive)) {
            tipo_texturas_disponiveis_ = 2;
        } else if ((has_difuse) && (has_especular) && (has_emissive)) {
            tipo_texturas_disponiveis_ = 3;
        }
    }

    void Material::bind_material_information(MapUniform& uniforms, std::vector<std::shared_ptr<Texture>>& v_tex) {
        // copy prop material
        uniforms.insert(list_material_.begin(), list_material_.end());

        // FIXME: seletorr de tipo ???
        uniforms[SHADE_TEXTURE_SELETOR_TIPO_VALIDO] = Uniform(tipo_texturas_disponiveis_);

        // indice de textura
        int index_tex = 0;
        for (const auto& kv : map_tex_) {
            v_tex.push_back(kv.second);
            uniforms[kv.first] = Uniform(index_tex);
            index_tex++;
        }
    }
} // namespace ce
