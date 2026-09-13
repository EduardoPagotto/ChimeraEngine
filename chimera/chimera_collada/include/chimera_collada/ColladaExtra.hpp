#pragma once
#include "Collada.hpp"

namespace ce {
    class ColladaExtra : public Collada {
      public:
        ColladaExtra(std::shared_ptr<entt::registry> registry, ColladaDom& dom, const std::string& url)
            : Collada(registry, dom, url) {};

        virtual ~ColladaExtra() {}
        void create(pugi::xml_node nodeExtra);
    };
} // namespace ce
