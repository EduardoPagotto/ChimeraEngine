#pragma once
#include "../Registry.hpp"
#include "Collada.hpp"

namespace Chimera {
class ColladaWaveFront : public Collada {
  public:
    ColladaWaveFront(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaWaveFront();
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);

  private:
};
} // namespace Chimera