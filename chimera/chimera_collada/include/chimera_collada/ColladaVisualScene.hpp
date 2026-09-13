#pragma once
#include "Collada.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {
    class ColladaVisualScene : public Collada {
      public:
        ColladaVisualScene(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {}
        virtual ~ColladaVisualScene() = default;
        void loadNode(pugi::xml_node node);
        void loadAll(pugi::xml_node node);

      private:
        void nodeData(pugi::xml_node node, Entity entity);
    };

} // namespace ce
