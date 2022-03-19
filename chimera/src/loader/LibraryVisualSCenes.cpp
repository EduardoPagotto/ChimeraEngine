#include "LibraryVisualSCenes.hpp"
#include "LibraryCameras.hpp"
#include "LibraryGeometrys.hpp"
#include "LibraryLights.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

void LibraryVisualScenes::target() {

    for (tinyxml2::XMLElement* l_nScene = root->FirstChildElement("library_visual_scenes")->FirstChildElement("visual_scene"); l_nScene;
         l_nScene = l_nScene->NextSiblingElement()) {

        std::string l_id = l_nScene->Attribute("id");
        if (url.compare(l_id) == 0) {

            for (tinyxml2::XMLElement* l_nNode = l_nScene->FirstChildElement("node"); l_nNode; l_nNode = l_nNode->NextSiblingElement()) {

                const char* l_idR = l_nNode->Attribute("id");
                const char* l_name = l_nNode->Attribute("name");
                const char* l_type = l_nNode->Attribute("type");

                tinyxml2::XMLElement* l_nDadoNode = l_nNode->FirstChildElement();
                if (l_nDadoNode != nullptr) {
                    // carregaNode(nullptr, l_nDadoNode, l_idR, l_name, l_type);
                    carregaNode(l_nDadoNode, l_idR, l_name, l_type);
                }
            }
            return; // pRootNode;
        }
    }

    throw std::string("Visual scenes nao encontrado: " + url);
}

glm::mat4 LibraryVisualScenes::getTransformation(tinyxml2::XMLElement* _nNode) {

    glm::mat4 l_pTransform;

    const char* l_tipoTransform = _nNode->Attribute("sid");
    if (strcmp(l_tipoTransform, (const char*)"transform") == 0) {
        l_pTransform = loadTransformMatrix(_nNode->GetText());
    } else {
        throw std::string("Matrix de transformacao invalida");
        // TODO: implementar carga de posicao, rotacao e transformar em matricial em
        // l_pTransform
    }

    return l_pTransform;
}

// void LibraryVisualScenes::carregaNode(Node* _pNodePai, tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char*
// type) {
void LibraryVisualScenes::carregaNode(tinyxml2::XMLElement* _nNode, const char* _id, const char* _name, const char* type) {

    glm::mat4 l_pTransform;
    // Node* pLastNodeDone = nullptr; // FIXME: quando for fazer o node

    for (_nNode; _nNode; _nNode = _nNode->NextSiblingElement()) {

        tinyxml2::XMLElement* l_nNodeSourceData = nullptr;

        const char* l_url = _nNode->Attribute("url");
        const char* l_target = _nNode->Attribute("target");
        const char* l_nomeElemento = _nNode->Value();

        if (strcmp(l_nomeElemento, (const char*)"matrix") == 0) {

            l_pTransform = getTransformation(_nNode);

        } else if (strcmp(l_nomeElemento, (const char*)"instance_camera") == 0) {

            Entity entity = reg->createEntity("Camera Entity");
            TransComponent& tc = entity.addComponent<TransComponent>();
            tc.trans = new Transform();
            tc.solid = false;
            tc.trans->setMatrix(l_pTransform);

            auto& tag = entity.getComponent<TagComponent>();
            tag.id = std::string(_id);

            LibraryCameras lib(root, l_url, entity);
            lib.target();

            // FIXME: Remover transformacao da camera e reposicionar na entidade
            CameraComponent& cc = entity.getComponent<CameraComponent>();
            cc.camera->setPosition(l_pTransform[3]);

            // FIXME: remover proximas versoes
            // pCamera->getCamera()->setPosition(l_pTransform[3]);
            // ICamera3D* pc = (ICamera3D*)pCamera->getCamera();
            // pc->updateDistanceFront();
            // _pNodePai->addChild(pCamera);
            // pLastNodeDone = pCamera;

        } else if (strcmp(l_nomeElemento, (const char*)"instance_light") == 0) {

            Entity entity = reg->createEntity("Light Entity");
            TransComponent& tc = entity.addComponent<TransComponent>();
            tc.trans = new Transform();
            tc.solid = false;
            tc.trans->setMatrix(l_pTransform);

            auto& tag = entity.getComponent<TagComponent>();
            tag.id = std::string(_id);

            LibraryLights lib(root, l_url, entity);
            lib.target();
            // pLight->data.setTransform(l_pTransform);
            //_pNodePai->addChild(pLight);
            // pLastNodeDone = pLight;

        } else if (strcmp(l_nomeElemento, (const char*)"instance_geometry") == 0) {

            Entity entity = reg->createEntity("Renderable Entity");
            TransComponent& tc = entity.addComponent<TransComponent>();
            tc.trans = new Transform();
            tc.solid = false;
            tc.trans->setMatrix(l_pTransform);

            auto& tag = entity.getComponent<TagComponent>();
            tag.id = std::string(_id);

            LibraryGeometrys lib(root, l_url, entity);
            lib.target();

            // pListNodes->mapMeshNode[pMesh->getName()] = pMesh;
            // pListNodes->mapMesh[std::string(_id)] = pMesh;
            // pMesh->setTransform(new Transform(l_pTransform));
            //_pNodePai->addChild(pMesh);
            // pLastNodeDone = _pNodePai; // pTrans;

        } else if (strcmp(l_nomeElemento, (const char*)"node") == 0) {

            // const char* l_id = _nNode->Attribute("id");
            // const char* l_name = _nNode->Attribute("name");
            // const char* l_type = _nNode->Attribute("type");
            // if (pLastNodeDone != nullptr) {
            //     carregaNode(pLastNodeDone, _nNode->FirstChildElement(), l_id, l_name, l_type);
            // } else {
            //     throw std::string("Falha, objeto hierarquia: " + std::string(l_id));
            // }

        } else {
            throw std::string("Falha, objeto desconhecido: " + std::string(l_nomeElemento));
        }
    }
}

} // namespace Chimera