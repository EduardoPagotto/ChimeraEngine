#pragma once
#include "Collada.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaGeometry : public Collada {
      public:
        ColladaGeometry(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {};
        virtual ~ColladaGeometry() = default;
        void create(Entity& entity, pugi::xml_node geo);
    };

} // namespace ce
