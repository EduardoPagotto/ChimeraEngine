#pragma once
#include "../Registry.hpp"
#include "../Texture.hpp"
#include "Collada.hpp"

namespace Chimera {
class ColladaImage : public Collada {
  public:
    ColladaImage(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaImage();
    void create(Entity entity, TexParam& tp, const pugi::xml_node& node);
};
} // namespace Chimera
