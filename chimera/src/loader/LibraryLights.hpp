#pragma once

#include "chimera/loader/Library.hpp"
#include "chimera/node/NodeLight.hpp"

namespace Chimera {

class LibraryLights : public Library {

  public:
    LibraryLights(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryLights();
    NodeLight* target();

  private:
    glm::vec4 getColor(tinyxml2::XMLElement* l_nColorVal);
    std::tuple<glm::vec4, LightType> loadDiffuseLightColor(tinyxml2::XMLElement* _nNode);
};
} // namespace Chimera