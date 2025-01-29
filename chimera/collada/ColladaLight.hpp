#pragma once
#include "Collada.hpp"
#include "core/Registry.hpp"

namespace ce {
class ColladaLight : public Collada {
  public:
    ColladaLight(ColladaDom& dom, const std::string& url) : Collada(dom, url) {};
    virtual ~ColladaLight() {}
    void create(Entity& entity, pugi::xml_node nodeLight);

  private:
};

} // namespace ce
