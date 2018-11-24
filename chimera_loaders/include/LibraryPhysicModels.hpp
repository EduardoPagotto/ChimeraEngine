#ifndef __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS__HPP
#define __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS__HPP

#include "Library.hpp"
#include "ListNodes.hpp"
#include "Solid.hpp"

namespace ChimeraLoaders {

class LibraryPhysicModels : public Library {

  public:
    LibraryPhysicModels(tinyxml2::XMLElement* _root, const std::string& _url,
                        Chimera::PhysicsControl* _pWorld);
    virtual ~LibraryPhysicModels();
    void target();

  private:
    void loadColladaShape(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape,
                          Chimera::Solid* _pPhysic);
    ListNodes* pListNodes;
    Chimera::PhysicsControl* pWorld;
};
} // namespace ChimeraLoaders

#endif