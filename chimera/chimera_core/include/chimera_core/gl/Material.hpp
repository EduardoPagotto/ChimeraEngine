#pragma once
#include "Texture.hpp"
#include "chimera_base/aux/Uniform.hpp"
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

    // FIXME: implementação futura
    // // Tipo alternativo para ID de textura (geralmente gerado por um Hash da string do caminho do arquivo)
    // using TextureId = uint32_t;

    // // Estrutura que define os parâmetros numéricos e vetoriais do material
    // struct MaterialProperties {
    //     float albedoColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // Cor base (RGBA)
    //     float metallic = 0.0f;                           // Grau de metalicidade (0 a 1)
    //     float roughness = 0.5f;                          // Rugosidade da superfície (0 a 1)
    //     float ao = 1.0f;                                 // Oclusão ambiental padrão
    //     float emissiveColor[3] = {0.0f, 0.0f, 0.0f};     // Cor de emissão de luz
    // };

    // // Estrutura principal do Material
    // struct Material {
    //     std::string name;  // Nome do material para debug e editor
    //     uint32_t shaderId; // ID do Shader/Pipeline que este material utiliza

    //     // Propriedades físicas básicas
    //     MaterialProperties properties;

    //     // IDs das texturas (0 ou um ID específico se não houver textura atribuída)
    //     TextureId albedoMapId = 0;
    //     TextureId normalMapId = 0;
    //     TextureId metallicMapId = 0;
    //     TextureId roughnessMapId = 0;
    //     TextureId aoMapId = 0;

    //     // Flags de renderização (Metadados de estado)
    //     bool isTransparent = false;
    //     bool isDoubleSided = false;
    //     bool castsShadows = true;

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
