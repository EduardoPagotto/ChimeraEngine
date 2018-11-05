#include "LibraryPhysicsScenes.h"
#include "ExceptionChimera.h"
#include "LoaderDaeUtils.h"

#include "LibraryPhysicModels.h"

namespace ChimeraLoaders {

LibraryPhysicsScenes::LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string &_url) : Library(_root, _url) {
}

LibraryPhysicsScenes::~LibraryPhysicsScenes() {
}

Chimera::Group *LibraryPhysicsScenes::target() {

    tinyxml2::XMLElement* l_nLib = root->FirstChildElement("library_physics_scenes");
    if ( l_nLib != nullptr ) {

        tinyxml2::XMLElement* l_nNodeBase = l_nLib->FirstChildElement ("physics_scene");
        if ( l_nNodeBase != nullptr ) {

            while ( l_nNodeBase != nullptr ) {
                std::string l_id = l_nNodeBase->Attribute ( "id" );
                if (url.compare(l_id) == 0) {

                    Chimera::Group *pGroup = new Chimera::Group(nullptr, l_id);

                    loadPhysicControlCollada(l_nNodeBase, nullptr);

                    tinyxml2::XMLElement* l_nPyModel = l_nNodeBase->FirstChildElement("instance_physics_model");
                    if (l_nPyModel != nullptr) {

                        const char *l_url = l_nNodeBase->Attribute("url");
                        if (l_url != nullptr) {

                            LibraryPhysicModels lib(root, l_url);
                            Chimera::Group *pGroupInner = lib.target();

                            tinyxml2::XMLElement* l_nRigid = l_nPyModel->FirstChildElement("instance_rigid_body");
                            while (l_nRigid != nullptr) {

                                const char *body = l_nRigid->Attribute("body");
                                const char *target = l_nRigid->Attribute("target");


                            // Chimera::Light *pLight = new Chimera::Light (nullptr,l_id );

                            // auto ret_data = loadDiffuseLightColor(l_nNodeBase);
                            // pLight->setDiffuse(std::get<0>(ret_data));
                            // pLight->setType(std::get<1>(ret_data));

                            // return pLight;   




                                l_nRigid = l_nRigid->NextSiblingElement();
                            }

                        } else {
                            throw Chimera::ExceptionChimera(Chimera::ExceptionCode::READ, "Sem url");
                        }    
                    }

                    return pGroup;
                }
                l_nNodeBase = l_nNodeBase->NextSiblingElement();
            }
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node: " + std::string ( url ) );
        } else {
            throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Node Tipo: " + std::string ( "physics_scene" ) );
        }
    } else {
        throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Falha, nao encontrado Library: " + std::string ( "library_physics_scenes" ) );
    }
}

void LibraryPhysicsScenes::loadPhysicControlCollada ( tinyxml2::XMLElement* _nNode, Chimera::PhysicsControl *_pPhysicsControl ) {

    tinyxml2::XMLElement* l_nTecnicCommon = _nNode->FirstChildElement ( "technique_common" );
    if ( l_nTecnicCommon != nullptr ) {

        tinyxml2::XMLElement* l_nNodeGravity = l_nTecnicCommon->FirstChildElement ( "gravity" );
        if ( l_nNodeGravity != nullptr ) {

            std::vector<btScalar> l_arrayF;
            const char* vetor = l_nNodeGravity->GetText();
            Chimera::loadArrayBtScalar ( vetor, l_arrayF );

            _pPhysicsControl->setGravity ( btVector3 ( l_arrayF[0], l_arrayF[1], l_arrayF[2] ) );

        }

        tinyxml2::XMLElement* l_nNodeStep = l_nTecnicCommon->FirstChildElement ( "time_step" );
        if ( l_nNodeGravity != nullptr ) {

            std::vector<btScalar> l_arrayF;
            const char* vetor = l_nNodeGravity->GetText();
            
            _pPhysicsControl->setStep( atof ( vetor) );
        }
    }
}

}