#include "SceneMng.h"

namespace Chimera {

    SceneMng::SceneMng(Node *_pRoot, Video *_pVideo)  {

		pRoot = _pRoot;
		parseEntity(pRoot);
		pCameraAtiva = nullptr;
        pObjeto = nullptr;

        pVideo = _pVideo;

	}

	SceneMng::~SceneMng() {

	}

	void SceneMng::parseEntity(Node *_pNode) {

		addEntityToScene(_pNode);

		for (Node *node : _pNode->listChild) {
			parseEntity(node);
		}

	}

	void SceneMng::addEntityToScene(Node *_pNode) {

		switch (_pNode->getKind()) {
		case EntityKind::CAMERA:
			m_vCamera.push_back((Camera*)_pNode);
			break;
		case EntityKind::LIGHT:
			m_vLight.push_back((Light*)_pNode);
			break;
		case EntityKind::OBJECT:
			m_vObject.push_back((Object*)_pNode);
			break;
        case EntityKind::PARTICLE_EMITTER:
            m_vParticle.push_back((ParticleEmitter*)_pNode);
            break;
        case EntityKind::SKYBOX:
            m_vSkyBox.push_back((SkyBox*)_pNode);
            break;
		default:
			break;
		}

	}

	void SceneMng::addChildToScene(Node *_pNode) {

		pRoot->addChild(_pNode);
		addEntityToScene(_pNode);

	}

	Node *SceneMng::getNode(EntityKind _type, const std::string &_nome) {

        Node *retorno = nullptr;

        switch (_type) {
            case EntityKind::CAMERA:
                for(Node *node : m_vCamera) {
                    if (node->getName().compare(_nome) == 0) {
                        return node;
                    }
                }
                break;
             case EntityKind::LIGHT:
                 for(Node *node : m_vLight) {
                     if (node->getName().compare(_nome) == 0) {
                         return node;
                     }
                 }
                 break;
             case EntityKind::OBJECT:
                 for(Node *node : m_vObject) {
                     if (node->getName().compare(_nome) == 0) {
                         return node;
                     }
                 }
                break;
            case EntityKind::PARTICLE_EMITTER:
                for(Node *node : m_vParticle) {
                    if (node->getName().compare(_nome) == 0) {
                        return node;
                    }
                }
                break;
            case EntityKind::SKYBOX:
                for(Node *node : m_vSkyBox) {
                    if (node->getName().compare(_nome) == 0) {
                        return node;
                    }
                }
                break;
            default:
                break;
        }

        return nullptr;

    }

	Node *SceneMng::getNode(EntityKind _type, unsigned index) {

		Node *retorno = nullptr;

		switch (_type) {
		case EntityKind::CAMERA:
			if (m_vCamera.size() > index)
                retorno = m_vCamera[index];
			break;
		case EntityKind::LIGHT:
			if (m_vLight.size() > index)
                retorno = m_vLight[index];
			break;
		case EntityKind::OBJECT:
			if (m_vObject.size() > index)
                retorno = m_vObject[index];
			break;
        case EntityKind::PARTICLE_EMITTER:
            if (m_vParticle.size() > index)
                retorno = m_vParticle[index];
            break;
        case EntityKind::SKYBOX:
            if (m_vSkyBox.size() > index)
                retorno = m_vSkyBox[index];
            break;
		default:
			break;
		}

		return retorno;
	}

	void SceneMng::execLight() {
		for (Light *pLight : m_vLight) {
			pLight->exec();
		}
	}

	void SceneMng::update(DataMsg *dataMsg) {

        if (dataMsg->getKindOp() == KindOp::START) {

            //inicialize primeiro os filhos para garantir textura e efeito em material
            pRoot->update(dataMsg);

            //inicializa objeto local
            shadoMap.init(pRoot);

        } else {

            pRoot->update(dataMsg);

        }

    }

    void SceneMng::draw(HUD *_pHud) {
//#define TESTEZ1
         int indiceDesenho = 1;
         if (pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS)
             indiceDesenho = 2;

#ifdef TESTEZ1
         for (Light *pLight : m_vLight) {
             btVector3 posicao = pLight->getPosition();
             shadoMap.StoreLightMatrices(posicao); //FIXME so funciona para 1 luz
         }

         shadoMap.RenderSceneA(pObjeto);
#endif

         for (int eye = 0; eye < indiceDesenho; eye++) {

             pVideo->executeViewPerspective(pCameraAtiva, eye);

             pCameraAtiva->exec();

             if (pSkyBox != nullptr)
                 pSkyBox->render(true);

             Chimera::DataMsg dataMsg(KindOp::DRAW3D, this, pObjeto, nullptr);
             update(&dataMsg);

             execLight();

#ifdef TESTEZ1
             shadoMap.ApplyShadowMap(pObjeto);
#endif

             if (pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS)
                 pVideo->updateHud(_pHud, 0);
             else
                 pVideo->updateHud(_pHud, eye);
         }

     }


    //  void SceneMng::draw3d() {
    //
    //      pCameraAtiva->exec();
    //
    //      if (pSkyBox != nullptr)
    //          pSkyBox->render();
    //
    //      Chimera::DataMsg dataMsg(KindOp::DRAW3D, this, pObjeto, nullptr);
    //      update(&dataMsg);
    //
    //         for (Light *pLight : m_vLight) {
    //             pLight->exec();
    //         }
    //
    //      //execLight();
    //
    //  }

} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
