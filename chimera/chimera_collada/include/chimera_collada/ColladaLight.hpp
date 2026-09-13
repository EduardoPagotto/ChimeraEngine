#pragma once
#include "Collada.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaLight : public Collada {
      public:
        ColladaLight(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {}
        virtual ~ColladaLight() = default;
        void create(Entity& entity, pugi::xml_node nodeLight);
    };

} // namespace ce
