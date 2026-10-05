#pragma once
#include "Collada.hpp"
#include "chimera_core/gl/TextureParams.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaEffect : public Collada {
      public:
        ColladaEffect(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {};

        virtual ~ColladaEffect() {
            mapa_tex_.clear();
            mapa2d_.clear();
        }
        void create(const std::string& ref_name, Entity& entity, pugi::xml_node node);

      private:
        void set_shader(const std::string& ref_name, const pugi::xml_node& node);
        bool set_texture_param(const pugi::xml_node& n, TexParam& tp);
        void set_image_parms(const pugi::xml_node& node);
        void set_material(const pugi::xml_node& node, TexParam& tp);
        Entity entity_;

        std::unordered_map<std::string, std::string> mapa_tex_;
        std::unordered_map<std::string, std::string> mapa2d_;
    };
} // namespace ce
