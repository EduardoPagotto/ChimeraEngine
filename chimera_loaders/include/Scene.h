#ifndef __CHIMERA_LOADER_SCENE_H
#define __CHIMERA_LOADER_SCENE_H

#include "Library.h"
#include "Group.h"
#include "PhysicsControl.h"
#include "TextureManager.h"
#include "ListNodes.h"

namespace ChimeraLoaders {

class Scene: public Library {

public:
    Scene(const std::string &_file);
    virtual ~Scene();
    std::tuple<Chimera::Group*,Chimera::PhysicsControl*> target();
private:
    Chimera::TextureManager *pTexManager;
    ListNodes *pListNodes;
};
}

#endif