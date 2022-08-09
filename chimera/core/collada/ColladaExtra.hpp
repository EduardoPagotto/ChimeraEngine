#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaExtra : public Collada {
  public:
    ColladaExtra(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaExtra() {}
    void create(Registry& r, pugi::xml_node nodeExtra);

  private:
};
} // namespace Chimera