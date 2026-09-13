#pragma once
#include "Collada.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaMesh : public Collada {
      public:
        ColladaMesh(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {}
        virtual ~ColladaMesh() = default;
        void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node nMesh);
    };
} // namespace ce
