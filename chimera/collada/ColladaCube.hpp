#pragma once
#include "Collada.hpp"
#include "core/Registry.hpp"

namespace ce {
class ColladaCube : public Collada {
  public:
    ColladaCube(ColladaDom& dom, const std::string& url) : Collada(dom, url) {};
    virtual ~ColladaCube() = default;
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);

  private:
};

} // namespace ce
