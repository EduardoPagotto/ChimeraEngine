#pragma once
#include "Collada.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaCam : public Collada {
      public:
        ColladaCam(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {};

        virtual ~ColladaCam() {}
        void create(Entity& entity, pugi::xml_node nodeCam);
        void createExtra(Entity& entity, pugi::xml_node node);
    };

} // namespace ce
