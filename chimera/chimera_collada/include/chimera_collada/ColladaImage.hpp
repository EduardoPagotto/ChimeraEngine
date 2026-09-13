#pragma once
#include "Collada.hpp"
#include "chimera_core/gl/Texture.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaImage : public Collada {
      public:
        ColladaImage(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {}
        virtual ~ColladaImage() = default;
        void create(Entity entity, TexParam& tp, const pugi::xml_node& node);
    };
} // namespace ce
