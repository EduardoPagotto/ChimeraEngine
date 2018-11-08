#include "Scene.h"
#include "ExceptionChimera.h"
#include "LibraryVisualSCenes.h"
#include "LibraryPhysicsScenes.h"

namespace ChimeraLoaders {

Scene::Scene(const std::string &_file) : Library(nullptr, "!" + _file) {
    pTexManager = Chimera::Singleton<Chimera::TextureManager>::getRefSingleton();
    pListNodes = Chimera::Singleton<ListNodes>::getRefSingleton();
}

Scene::~Scene() {
    Chimera::Singleton<Chimera::TextureManager>::releaseRefSingleton();
    Chimera::Singleton<ListNodes>::releaseRefSingleton();
}

std::tuple<Chimera::Group*,Chimera::PhysicsControl*> Scene::target() {

    Chimera::Group *pGroup = nullptr;
    Chimera::PhysicsControl *pPhysicsControl = nullptr;

    tinyxml2::XMLElement* l_nVisual = root->FirstChildElement("scene")->FirstChildElement("instance_visual_scene");
    tinyxml2::XMLElement* l_nPhysic = root->FirstChildElement("scene")->FirstChildElement("instance_physics_scene");

    if (l_nVisual) {
        std::string l_url = l_nVisual->Attribute("url");

        LibraryVisualScenes lib(root, l_url);
        pGroup = lib.target();
    }

    if (l_nPhysic) {
         std::string l_url = l_nPhysic->Attribute("url");

        LibraryPhysicsScenes lib(root, l_url);
        pPhysicsControl = lib.target();
    }

    return std::make_tuple(pGroup, pPhysicsControl);
}
}