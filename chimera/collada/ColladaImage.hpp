#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/gl/Texture.hpp"

namespace ce {
class ColladaImage : public Collada {
  public:
    ColladaImage(ColladaDom& dom, const std::string& url) : Collada(dom, url) {}
    virtual ~ColladaImage() = default;
    void create(Entity entity, TexParam& tp, const pugi::xml_node& node);
};
} // namespace ce
