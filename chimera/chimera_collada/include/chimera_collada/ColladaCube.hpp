#pragma once
#include "Collada.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaCube : public Collada {
      public:
        ColladaCube(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {};
        virtual ~ColladaCube() {}
        void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);
    };

} // namespace ce
