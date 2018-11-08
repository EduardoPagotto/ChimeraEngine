#include "Scene.h"
#include "ExceptionChimera.h"
#include "LibraryVisualSCenes.h"
#include "LibraryPhysicsScenes.h"

namespace ChimeraLoaders {

Scene::Scene(const std::string &_file) : Library(nullptr, "!" + _file) {
    pTexManager = Chimera::Singleton<Chimera::TextureManager>::getRefSingleton();
}

Scene::~Scene() {
    Chimera::Singleton<Chimera::TextureManager>::releaseRefSingleton();
}

Chimera::Node *Scene::target() {

    tinyxml2::XMLElement* l_nVisual = root->FirstChildElement("scene")->FirstChildElement("instance_visual_scene");
    tinyxml2::XMLElement* l_nPhysic = root->FirstChildElement("scene")->FirstChildElement("instance_physics_scene");

    if (l_nVisual) {
        std::string l_url = l_nVisual->Attribute("url");

        LibraryVisualScenes lib(root, l_url);
        pGroup = lib.target();
    }

    if (l_nPhysic) {
         std::string l_url = l_nPhysic->Attribute("url");

        LibraryPhysicsScenes lib(root, l_url, pGroup);
        pPhysicsControl = lib.target();
    }
    return pGroup;
}
}