#pragma once
#include "Collada.hpp"

namespace ce {
    class ColladaPhysicScene : public Collada {
      public:
        ColladaPhysicScene(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {};
        virtual ~ColladaPhysicScene() = default;
        const pugi::xml_node find_model(pugi::xml_node node, const std::string& body);
        void load_all(pugi::xml_node node);
    };
} // namespace ce
