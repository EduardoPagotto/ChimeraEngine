#include "LibraryPhysicsScenes.hpp"
#include "LibraryPhysicModels.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/render/Transform.hpp"
#include "chimera/render/bullet/Solid.hpp"

namespace Chimera {

void LibraryPhysicsScenes::target() {

    tinyxml2::XMLElement* l_nPhyScene = root->FirstChildElement("library_physics_scenes")->FirstChildElement("physics_scene");
    for (l_nPhyScene; l_nPhyScene; l_nPhyScene = l_nPhyScene->NextSiblingElement()) {

        std::string l_id = l_nPhyScene->Attribute("id");
        if (url.compare(l_id) == 0) {

            loadPhysicControlCollada(l_nPhyScene);

            tinyxml2::XMLElement* l_nPyModel = l_nPhyScene->FirstChildElement("instance_physics_model");
            if (l_nPyModel != nullptr) {

                std::string l_url = l_nPyModel->Attribute("url");

                std::map<std::string, Solid*> mapSolids;
                tinyxml2::XMLElement* l_nRigid = l_nPyModel->FirstChildElement("instance_rigid_body");
                for (l_nRigid; l_nRigid; l_nRigid = l_nRigid->NextSiblingElement()) {

                    std::string body = l_nRigid->Attribute("body");
                    std::string target = l_nRigid->Attribute("target");
                    LibraryPhysicModels lib(root, l_url);
                    lib.target2(body, getIdFromUrl(target), reg);
                }
                mapSolids.clear();
            }
            return;
        }
    }
    throw Exception("Physics scene nao encontrado: " + url);
}

void LibraryPhysicsScenes::loadPhysicControlCollada(tinyxml2::XMLElement* _nNode) {

    tinyxml2::XMLElement* l_nNodeGravity = _nNode->FirstChildElement("technique_common")->FirstChildElement("gravity");
    if (l_nNodeGravity != nullptr) {

        std::vector<btScalar> l_arrayF;
        const char* vetor = l_nNodeGravity->GetText();
        loadArrayBtScalar(vetor, l_arrayF);

        entity = reg->createEntity("physic_master");
        PhysicsControl& pc = entity.addComponent<PhysicsControl>();
        pc.setGravity(btVector3(l_arrayF[0], l_arrayF[1], l_arrayF[2]));
    }

    // tinyxml2::XMLElement* l_nNodeStep = _nNode->FirstChildElement("technique_common")->FirstChildElement("time_step");
    // if (l_nNodeStep != nullptr) {
    //     const char* vetor = l_nNodeStep->GetText();
    //     pPhysicsControl->setStep(atof(vetor));
    // }
}
} // namespace Chimera