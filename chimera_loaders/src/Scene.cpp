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

    tinyxml2::XMLElement* child = root->FirstChildElement("scene")->FirstChildElement();
    for(child;  child;  child = child->NextSiblingElement()) {

        const char* valElemet = child->Value();

        if (child->ToElement()->Attribute("url")) {

            std::string l_url = child->ToElement()->Attribute("url");

            if (strcmp(valElemet, (const char*)"instance_visual_scene") == 0) {

                LibraryVisualScenes lib(root, l_url);
                pGroup = lib.target();
                
            } else if (strcmp(valElemet, (const char*)"instance_physics_scene") == 0) {
                
                LibraryPhysicsScenes lib(root, l_url, pGroup);
                pPhysicsControl = lib.target();
            }
        }
    }
    return pGroup;
}
}