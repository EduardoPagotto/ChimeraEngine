#pragma once
#include "Collada.hpp"
#include "chimera/ecs/Entity.hpp"

namespace ce {
    class ColladaHeightMap : public Collada {
      public:
        ColladaHeightMap(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
        virtual ~ColladaHeightMap() {}
        void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);
    };
} // namespace ce
