#pragma once
#include "Collada.hpp"
#include "chimera/ecs/Entity.hpp"

namespace ce {
    class ColladaMesh : public Collada {
      public:
        ColladaMesh(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
        virtual ~ColladaMesh() = default;
        void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node nMesh);
    };
} // namespace ce
