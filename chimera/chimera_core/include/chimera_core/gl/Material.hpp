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
        void set_default_effect();
        void add_texture(const std::string& uniform_tex_name, std::shared_ptr<Texture> texture) {
            this->map_tex_[uniform_tex_name] = texture;
        }
        inline void set_ambient(const glm::vec4& color) { list_material_[SHADE_MAT_AMBIENTE] = Uniform(color); }
        inline void set_specular(const glm::vec4& color) { list_material_[SHADE_MAT_SPECULA] = Uniform(color); }
        inline void set_diffuse(const glm::vec4& color) { list_material_[SHADE_MAT_DIFFUSE] = Uniform(color); }
        inline void set_emission(const glm::vec4& color) { list_material_[SHADE_MAT_EMISSIVE] = Uniform(color); }
        inline void set_shine(const float& val) { list_material_[SHADE_MAT_SHININESS] = Uniform(val); }

        bool has_texture() { return !map_tex_.empty(); }
        void bind_material_information(MapUniform& uniforms, std::vector<std::shared_ptr<Texture>>& v_tex);
        bool const is_valid() const { return valid_; }

      private:
        bool valid_;
        int tipo_texturas_disponiveis_;
        std::unordered_map<std::string, std::shared_ptr<Texture>> map_tex_;
        MapUniform list_material_;
    };
} // namespace ce
