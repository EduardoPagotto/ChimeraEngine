#pragma once
#include "Collada.hpp"

namespace Chimera {
class ColladaExtra : public Collada {
  public:
    ColladaExtra(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaExtra() {}
    void create(pugi::xml_node nodeExtra);

  private:
};
} // namespace Chimera