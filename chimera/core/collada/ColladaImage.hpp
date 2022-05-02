#pragma once
#include "Collada.hpp"
#include "chimera/core/visible/Texture.hpp"
#include <unordered_map>

namespace Chimera {
class ColladaImage : public Collada {
  public:
    ColladaImage(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    void create(pugi::xml_node nodeParent);
    virtual ~ColladaImage();
    Texture* getTexureByName(const std::string& name);

  private:
    void loadImage(const std::string& id);

    std::unordered_map<std::string, std::string> mapaTex;
    std::unordered_map<std::string, std::string> mapa2D;
};
} // namespace Chimera
