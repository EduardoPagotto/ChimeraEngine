#pragma once
#include "Collada.hpp"
#include "chimera/ecs/Entity.hpp"

namespace ce {
    class ColladaMaterial : public Collada {
      public:
        ColladaMaterial(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
        virtual ~ColladaMaterial() = default;
        void create(Entity& entity, const pugi::xml_node& node);
    };
} // namespace ce
