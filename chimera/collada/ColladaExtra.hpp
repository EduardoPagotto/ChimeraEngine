#pragma once
#include "Collada.hpp"

namespace Chimera {
class ColladaExtra : public Collada {
  public:
    ColladaExtra(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc) : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaExtra() {}
    void create(pugi::xml_node nodeExtra);

  private:
};
} // namespace Chimera