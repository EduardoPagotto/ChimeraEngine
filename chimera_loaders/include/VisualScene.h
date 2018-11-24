#ifndef __CHIMERA_LOADER_VISUAL_SCENE__HPP
#define __CHIMERA_LOADER_VISUAL_SCENE__HPP

#include "Group.h"
#include "Library.h"
#include "ListNodes.h"
#include "PhysicsControl.h"
#include "TextureManager.h"

namespace ChimeraLoaders {

class VisualScene : public Library {

  public:
    VisualScene(const std::string& _file, Chimera::Group* _pGroup);
    virtual ~VisualScene();
    void target();

  private:
    Chimera::TextureManager* pTexManager;
    ListNodes* pListNodes;
    Chimera::Group* pGroup;
};
} // namespace ChimeraLoaders

#endif