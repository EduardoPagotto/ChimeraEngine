#pragma once
#include "chimera/core/Registry.hpp"
#include "chimera/loader/Library.hpp"

namespace Chimera {

class LibraryVisualScenes : public Library {

  public:
    LibraryVisualScenes(tinyxml2::XMLElement* _root, const std::string& _url, Registry* reg) : Library(_root, _url), reg(reg) {}
    virtual ~LibraryVisualScenes(){};
    void target();

  private:
    glm::mat4 getTransformation(tinyxml2::XMLElement* _nNode);
    void carregaNode(tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type);
    Registry* reg;
};
} // namespace Chimera
