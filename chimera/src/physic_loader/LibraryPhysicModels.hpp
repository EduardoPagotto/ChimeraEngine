#ifndef __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS__HPP
#define __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/physic/Solid.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class LibraryPhysicModels : public Library {

  public:
    LibraryPhysicModels(tinyxml2::XMLElement* _root, const std::string& _url, PhysicsControl* _pWorld);
    virtual ~LibraryPhysicModels();
    void target(std::map<std::string, Solid*>& _mapSolids);
    void target2(const std::string& body, const std::string target, Scene* scene); // body, target, scene, entityPC);

  private:
    void loadColladaShape(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, Solid* _pPhysic);
    ListNodes* pListNodes;
    PhysicsControl* pWorld;
};
} // namespace Chimera

#endif