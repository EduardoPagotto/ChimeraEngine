#pragma once

#include "chimera/loader/Library.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

class LibraryLights : public Library {

  public:
    LibraryLights(tinyxml2::XMLElement* _root, const std::string& _url, Entity entity) : Library(_root, _url), entity(entity) {}
    virtual ~LibraryLights() {}
    void target();

  private:
    glm::vec4 getColor(tinyxml2::XMLElement* l_nColorVal);
    std::tuple<glm::vec4, LightType> loadDiffuseLightColor(tinyxml2::XMLElement* _nNode);
    Entity entity;
};
} // namespace Chimera