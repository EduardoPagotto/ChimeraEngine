#pragma once
#include "Collada.hpp"
#include "chimera/ecs/Entity.hpp"

namespace ce {
    class ColladaVisualScene : public Collada {
      public:
        ColladaVisualScene(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
        virtual ~ColladaVisualScene() = default;
        void loadNode(pugi::xml_node node);
        void loadAll(pugi::xml_node node);

      private:
        void nodeData(pugi::xml_node node, Entity entity);
    };

} // namespace ce
