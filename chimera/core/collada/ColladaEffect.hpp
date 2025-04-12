#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/TextureMng.hpp"

namespace ce {
class ColladaEffect : public Collada {
  public:
    ColladaEffect(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc) : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaEffect() {
        mapaTex.clear();
        mapa2D.clear();
    }
    void create(const std::string& refName, Entity& entity, pugi::xml_node node);

  private:
    void setShader(const std::string& refName, const pugi::xml_node& node);
    bool setTextureParam(const pugi::xml_node& n, TexParam& tp);
    void setImageParms(const pugi::xml_node& node);
    void setMaterial(const pugi::xml_node& node, TexParam& tp);
    Entity entity;

    std::unordered_map<std::string, std::string> mapaTex;
    std::unordered_map<std::string, std::string> mapa2D;
};
} // namespace ce
