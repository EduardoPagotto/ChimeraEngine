#pragma once
#include "Collada.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaWaveFront : public Collada {
      public:
        ColladaWaveFront(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {}

        virtual ~ColladaWaveFront() = default;
        void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);
    };
} // namespace ce
