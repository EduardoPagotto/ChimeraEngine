#pragma once
#include "Collada.hpp"
#include "chimera/ecs/Entity.hpp"

namespace ce {
    class ColladaCam : public Collada {
      public:
        ColladaCam(ColladaDom& dom, const std::string& url) : Collada(dom, url) {};
        virtual ~ColladaCam() {}
        void create(Entity& entity, pugi::xml_node nodeCam);
        void createExtra(Entity& entity, pugi::xml_node node);
    };

} // namespace ce
