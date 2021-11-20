#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/render/bullet/Solid.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class LibraryPhysicModels : public Library {

  public:
    LibraryPhysicModels(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryPhysicModels();
    void target(std::map<std::string, Solid*>& _mapSolids);
    void target2(const std::string& body, const std::string target, Scene* scene);

  private:
    void loadColladaShape(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, Solid* _pPhysic);
    ListNodes* pListNodes;
};
} // namespace Chimera
