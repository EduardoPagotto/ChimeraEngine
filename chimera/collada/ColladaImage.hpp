#pragma once
#include "Collada.hpp"
#include "chimera/ecs/Entity.hpp"
#include "chimera_core/gl/Texture.hpp"

namespace ce {
    class ColladaImage : public Collada {
      public:
        ColladaImage(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
        virtual ~ColladaImage() = default;
        void create(Entity entity, TexParam& tp, const pugi::xml_node& node);
    };
} // namespace ce
