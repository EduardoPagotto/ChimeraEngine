#ifndef __CHIMERA_LOADER_VISUAL_SCENE_H
#define __CHIMERA_LOADER_VISUAL_SCENE_H

#include "Library.h"
#include "Group.h"
#include "PhysicsControl.h"
#include "TextureManager.h"
#include "ListNodes.h"

namespace ChimeraLoaders {

class VisualScene: public Library {

public:
    VisualScene(const std::string &_file, Chimera::Group *_pGroup);
    virtual ~VisualScene();
    void target();
private:
    Chimera::TextureManager *pTexManager;
    ListNodes *pListNodes;
    Chimera::Group *pGroup;
};
}

#endif