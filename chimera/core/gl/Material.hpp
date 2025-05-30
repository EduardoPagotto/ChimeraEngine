#pragma once
#include "Texture.hpp"
#include "chimera/base/Uniform.hpp"
#include <vector>

namespace ce {

#define SHADE_TEXTURE_SELETOR_TIPO_VALIDO "tipo"
#define SHADE_MAT_AMBIENTE                "material.ambient"
#define SHADE_MAT_DIFFUSE                 "material.diffuse"
#define SHADE_MAT_SPECULA                 "material.specular"
#define SHADE_MAT_EMISSIVE                "material.emissive"
#define SHADE_MAT_SHININESS               "material.shininess"
#define SHADE_TEXTURE_DIFFUSE             "material.tDiffuse"
#define SHADE_TEXTURE_SPECULA             "material.tSpecular"
#define SHADE_TEXTURE_EMISSIVE            "material.tEmissive"

    class Material {
      public:
        Material();
        virtual ~Material();
        void init();
        void setDefaultEffect();
        void addTexture(const std::string& uniformTexName, std::shared_ptr<Texture> texture) {
            this->mapTex[uniformTexName] = texture;
        }
        inline void setAmbient(const glm::vec4& _color) { listMaterial[SHADE_MAT_AMBIENTE] = Uniform(_color); }
        inline void setSpecular(const glm::vec4& _color) { listMaterial[SHADE_MAT_SPECULA] = Uniform(_color); }
        inline void setDiffuse(const glm::vec4& _color) { listMaterial[SHADE_MAT_DIFFUSE] = Uniform(_color); }
        inline void setEmission(const glm::vec4& _color) { listMaterial[SHADE_MAT_EMISSIVE] = Uniform(_color); }
        inline void setShine(const float& _val) { listMaterial[SHADE_MAT_SHININESS] = Uniform(_val); }

        bool hasTexture() { return !mapTex.empty(); }
        void bindMaterialInformation(MapUniform& uniforms, std::vector<std::shared_ptr<Texture>>& vTex);
        bool const isValid() const { return valid; }

      private:
        bool valid;
        int tipoTexturasDisponiveis;
        std::unordered_map<std::string, std::shared_ptr<Texture>> mapTex;
        MapUniform listMaterial;
    };
} // namespace ce
