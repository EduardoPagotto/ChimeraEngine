#pragma once
#include "Collada.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaMaterial : public Collada {
      public:
        ColladaMaterial(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {}

        virtual ~ColladaMaterial() = default;
        void create(Entity& entity, const pugi::xml_node& node);
    };
} // namespace ce
