#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/TextureManager.hpp"

namespace Chimera {
class ColladaEffect : public Collada {
  public:
    ColladaEffect(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaEffect();
    void create(const std::string& refName, Entity& entity, pugi::xml_node node);

  private:
    void setShader(const std::string& refName, const pugi::xml_node& node);
    bool setTextureParam(const pugi::xml_node& n, TexParam& tp);
    void setImageParms(const pugi::xml_node& node);
    void setMaterial(const pugi::xml_node& node);
    Entity entity;

    std::unordered_map<std::string, std::string> mapaTex;
    std::unordered_map<std::string, std::string> mapa2D;
};
} // namespace Chimera
