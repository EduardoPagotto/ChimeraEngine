#pragma once
#include "Collada.hpp"
#include "Registry.hpp"

namespace Chimera {
class ColladaWaveFront : public Collada {
  public:
    ColladaWaveFront(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaWaveFront();
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);

  private:
};
} // namespace Chimera