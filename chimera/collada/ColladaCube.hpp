#pragma once
#include "Collada.hpp"
#include "core/Registry.hpp"

namespace ce {
class ColladaCube : public Collada {
  public:
    ColladaCube(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc) : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaCube() {}
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);

  private:
};

} // namespace ce
