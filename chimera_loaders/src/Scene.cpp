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


    for(tinyxml2::XMLElement* child = root->FirstChildElement("scene")->FirstChildElement();  child != nullptr;  child = child->NextSiblingElement()) {

        const char* val = child->Value();

        if (child->ToElement()->Attribute("url")) {

            std::string l_url = child->ToElement()->Attribute("url");

            if (strcmp(val, (const char*)"instance_visual_scene") == 0) {
                LibraryVisualScenes lib(root, l_url);
                pGroup = lib.target();
            } else if (strcmp(val, (const char*)"instance_physics_scene") == 0) {
                LibraryPhysicsScenes lib(root, l_url, pGroup);
                pPhysicsControl = lib.target();
            }
        }
            
    } 


    // tinyxml2::XMLElement* l_nLib = root->FirstChildElement("scene");
    // if ( l_nLib != nullptr ) {

    //     //Chimera::Group *vs = nullptr;

    //     tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("instance_visual_scene");
    //     if ( l_nNodeBase != nullptr ) {
    //         std::string l_url = l_nNodeBase->Attribute ( "url" );

    //         LibraryVisualScenes lib(root, l_url);
    //         pGroup = lib.target();
    //     }

    //     l_nNodeBase = l_nLib->FirstChildElement ("instance_physics_scene");
    //     if ( l_nNodeBase != nullptr ) {
    //         std::string l_url = l_nNodeBase->Attribute ( "url" );

    //         LibraryPhysicsScenes lib(root, l_url, pGroup);
    //         pPhysicsControl = lib.target();
    //     }

        return pGroup;

    // } else {
    //     throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "scene" ) );
    // }
}
}