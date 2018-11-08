#include "LibraryPhysicsScenes.h"
#include "ExceptionChimera.h"
#include "LoaderDaeUtils.h"

#include "LibraryPhysicModels.h"

#include "Solid.h"

namespace ChimeraLoaders {

LibraryPhysicsScenes::LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string &_url, Chimera::Group *_pScene) : Library(_root, _url) {
    pScene = _pScene;
}

LibraryPhysicsScenes::~LibraryPhysicsScenes() {
}

Chimera::PhysicsControl *LibraryPhysicsScenes::target() {

    tinyxml2::XMLElement* l_nPhyScene = root->FirstChildElement("library_physics_scenes")->FirstChildElement ("physics_scene");
    for(l_nPhyScene; l_nPhyScene; l_nPhyScene=l_nPhyScene->NextSiblingElement()) {

        std::string l_id = l_nPhyScene->Attribute ( "id" );
        if (url.compare(l_id) == 0) {

            //Chimera::Group *pGroup = new Chimera::Group(nullptr, l_id);
            // FIXME: carga com nome do l_id
            Chimera::PhysicsControl *pPhysicsControl = new Chimera::PhysicsControl();

            loadPhysicControlCollada(l_nPhyScene, pPhysicsControl);

            tinyxml2::XMLElement* l_nPyModel = l_nPhyScene->FirstChildElement("instance_physics_model");
            if (l_nPyModel != nullptr) {

                std::string l_url = l_nPyModel->Attribute("url");

                LibraryPhysicModels lib(root, l_url);
                Chimera::Group *pGroupInner = lib.target();

                tinyxml2::XMLElement* l_nRigid = l_nPyModel->FirstChildElement("instance_rigid_body");
                for(l_nRigid; l_nRigid; l_nRigid=l_nRigid->NextSiblingElement()) {

                    std::string body = l_nRigid->Attribute("body");
                    std::string target = l_nRigid->Attribute("target");

                    Chimera::Solid *pSolid = (Chimera::Solid*)pGroupInner->findChild(body, false);
                    Chimera::Node *node = pScene->findChild(getIdFromUrl(target), true); 

                    Chimera::Node *transforfParent = node->getParent();
                    Chimera::Node *mesh = node->findChild(Chimera::EntityKind::MESH, 0, false);

                    transforfParent->addChild(pSolid);
                    mesh->setParent(pSolid);

                    transforfParent->removeChild(node);

                    //Chimera::Node *pai = transforOld->getParent();
                    //->removeChild(transforOld);
                    //pai->addChild(pSolid);
                    //pSolid->addChild(node);
                }
            }
            return pPhysicsControl;
        }
    }
    throw Chimera::ExceptionChimera ( Chimera::ExceptionCode::READ, "Physics scene nao encontrado: " + url);
}

void LibraryPhysicsScenes::loadPhysicControlCollada ( tinyxml2::XMLElement* _nNode, Chimera::PhysicsControl *_pPhysicsControl ) {

        tinyxml2::XMLElement* l_nNodeGravity = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "gravity" );
        if ( l_nNodeGravity != nullptr ) {

            std::vector<btScalar> l_arrayF;
            const char* vetor = l_nNodeGravity->GetText();
            Chimera::loadArrayBtScalar ( vetor, l_arrayF );
            _pPhysicsControl->setGravity ( btVector3 ( l_arrayF[0], l_arrayF[1], l_arrayF[2] ) );

        }

        tinyxml2::XMLElement* l_nNodeStep = _nNode->FirstChildElement ( "technique_common" )->FirstChildElement ( "time_step" );
        if ( l_nNodeStep != nullptr ) {

            const char* vetor = l_nNodeStep->GetText();
            _pPhysicsControl->setStep( atof ( vetor) );

        }
}
}