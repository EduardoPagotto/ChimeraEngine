#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class LibraryVisualScenes : public Library {

  public:
    LibraryVisualScenes(tinyxml2::XMLElement* _root, const std::string& _url, Scene* scene) : Library(_root, _url), scene(scene) {}
    virtual ~LibraryVisualScenes(){};
    void target();

  private:
    glm::mat4 getTransformation(tinyxml2::XMLElement* _nNode);
    void carregaNode(tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type);
    Scene* scene;
};
} // namespace Chimera
