#pragma once
#include "Collada.hpp"
#include "chimera/ecs/Entity.hpp"

namespace ce {
    class ColladaGeometry : public Collada {
      public:
        ColladaGeometry(ColladaDom& dom, const std::string& url) : Collada(dom, url) {};
        virtual ~ColladaGeometry() = default;
        void create(Entity& entity, pugi::xml_node geo);
    };

} // namespace ce
