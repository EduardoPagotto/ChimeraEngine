#include "chimera/core/Exception.hpp"

#include "LibraryPhysicModels.hpp"
#include "LibraryPhysicsScenes.hpp"

#include "chimera/node/Solid.hpp"
#include "chimera/node/Transform.hpp"

namespace ChimeraLoaders {

LibraryPhysicsScenes::LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string& _url,
                                           Chimera::PhysicsControl* _pPhysicsControl)
    : Library(_root, _url) {
    pListNodes = Chimera::Singleton<ListNodes>::getRefSingleton();
    pPhysicsControl = _pPhysicsControl;
}

LibraryPhysicsScenes::~LibraryPhysicsScenes() { Chimera::Singleton<ListNodes>::releaseRefSingleton(); }

void LibraryPhysicsScenes::target() {

    tinyxml2::XMLElement* l_nPhyScene =
        root->FirstChildElement("library_physics_scenes")->FirstChildElement("physics_scene");
    for (l_nPhyScene; l_nPhyScene; l_nPhyScene = l_nPhyScene->NextSiblingElement()) {

        std::string l_id = l_nPhyScene->Attribute("id");
        if (url.compare(l_id) == 0) {

            loadPhysicControlCollada(l_nPhyScene);

            tinyxml2::XMLElement* l_nPyModel = l_nPhyScene->FirstChildElement("instance_physics_model");
            if (l_nPyModel != nullptr) {

                std::string l_url = l_nPyModel->Attribute("url");

                LibraryPhysicModels lib(root, l_url, pPhysicsControl);
                lib.target();

                tinyxml2::XMLElement* l_nRigid = l_nPyModel->FirstChildElement("instance_rigid_body");
                for (l_nRigid; l_nRigid; l_nRigid = l_nRigid->NextSiblingElement()) {

                    std::string body = l_nRigid->Attribute("body");
                    std::string target = l_nRigid->Attribute("target");

                    Chimera::Solid* pSolid =
                        (Chimera::Solid*)pListNodes->getByName(Chimera::EntityKind::SOLID,
                                                               getIdFromUrl(body)); // ->findChild(body, false);
                    Chimera::Node* node = pListNodes->getByName(Chimera::EntityKind::TRANSFORM, getIdFromUrl(target));
                    Chimera::Node* transforfParent = node->getParent();
                    Chimera::Node* mesh = node->findChild(Chimera::EntityKind::MESH, 0, false);

                    // Transfere dados
                    Chimera::Transform* pTrans = (Chimera::Transform*)node;
                    pSolid->setMatrix(pTrans->getMatrix());

                    transforfParent->addChild(pSolid);
                    pSolid->addChild(mesh);
                    // mesh->setParent(pSolid);

                    transforfParent->removeChild(node);

                    // Chimera::Node *pai = transforOld->getParent();
                    //->removeChild(transforOld);
                    // pai->addChild(pSolid);
                    // pSolid->addChild(node);
                }
            }
            return;
        }
    }
    throw Chimera::Exception("Physics scene nao encontrado: " + url);
}

void LibraryPhysicsScenes::loadPhysicControlCollada(tinyxml2::XMLElement* _nNode) {

    tinyxml2::XMLElement* l_nNodeGravity = _nNode->FirstChildElement("technique_common")->FirstChildElement("gravity");
    if (l_nNodeGravity != nullptr) {

        std::vector<btScalar> l_arrayF;
        const char* vetor = l_nNodeGravity->GetText();
        loadArrayBtScalar(vetor, l_arrayF);
        pPhysicsControl->setGravity(btVector3(l_arrayF[0], l_arrayF[1], l_arrayF[2]));
    }

    tinyxml2::XMLElement* l_nNodeStep = _nNode->FirstChildElement("technique_common")->FirstChildElement("time_step");
    if (l_nNodeStep != nullptr) {

        const char* vetor = l_nNodeStep->GetText();
        pPhysicsControl->setStep(atof(vetor));
    }
}
} // namespace ChimeraLoaders