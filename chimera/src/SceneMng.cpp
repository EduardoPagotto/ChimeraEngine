#include "SceneMng.h"

#include "OpenGLDefs.h"

namespace Chimera {

SceneMng::SceneMng ( Video *_pVideo ) : pVideo(_pVideo) {

    root = new SceneRoot(nullptr,"root");
    pCameraAtiva = nullptr;
    pObjeto = nullptr;
    pSkyBox = nullptr;
    pLoader = nullptr;
}

SceneMng::~SceneMng() {
}

void SceneMng::setReader ( LoaderDae* _pLoader ) {

    this->pLoader = _pLoader;
}

Group *SceneMng::createSceneGraph() {
   
    Group *pGroup = pLoader->getNodes();
    root->addChild(pGroup);
    parseEntity(pGroup);
    
    return pGroup;
}

void SceneMng::parseEntity ( Node *_pNode ) {

    addEntityToScene ( _pNode );

	std::vector<Node*>* vList  = _pNode->getChilds();

	if (vList != nullptr) {
		for (int indice = 0; indice < vList->size(); indice++) {

			Node* pNode = vList->at(indice);
			parseEntity(pNode);
		}
	}
}

void SceneMng::addEntityToScene ( Node *_pNode ) {

    switch ( _pNode->getKind() ) {
    case EntityKind::CAMERA:
        m_vCamera.push_back ( ( Camera* ) _pNode );
        break;
    case EntityKind::LIGHT:
        m_vLight.push_back ( ( Light* ) _pNode );
        break;
    case EntityKind::OBJECT:
        m_vObject.push_back ( ( Object* ) _pNode );
        break;
    case EntityKind::PARTICLE_EMITTER:
        m_vParticle.push_back ( ( ParticleEmitter* ) _pNode );
        break;
    case EntityKind::SKYBOX:
       m_vSkyBox.push_back ( ( SkyBox* ) _pNode );
       break;
    default:
        break;
    }

}

Node *SceneMng::getNode ( EntityKind _type, const std::string &_nome ) {

    Node *retorno = nullptr;

    switch ( _type ) {
    case EntityKind::CAMERA:
        for ( Node *node : m_vCamera ) {
            if ( node->getName().compare ( _nome ) == 0 ) {
                return node;
            }
        }
        break;
    case EntityKind::LIGHT:
        for ( Node *node : m_vLight ) {
            if ( node->getName().compare ( _nome ) == 0 ) {
                return node;
            }
        }
        break;
    case EntityKind::OBJECT:
        for ( Node *node : m_vObject ) {
            if ( node->getName().compare ( _nome ) == 0 ) {
                return node;
            }
        }
        break;
    case EntityKind::PARTICLE_EMITTER:
        for ( Node *node : m_vParticle ) {
            if ( node->getName().compare ( _nome ) == 0 ) {
                return node;
            }
        }
        break;
        //FIXME verificar como implementar
    case EntityKind::SKYBOX:
        for ( Node *node : m_vSkyBox ) {
            if ( node->getName().compare ( _nome ) == 0 ) {
                return node;
            }
        }
        break;
    default:
        break;
    }

    return nullptr;

}

Node *SceneMng::getNode ( EntityKind _type, unsigned index ) {

    Node *retorno = nullptr;

    switch ( _type ) {
    case EntityKind::CAMERA:
        if ( m_vCamera.size() > index ) {
            retorno = m_vCamera[index];
        }
        break;
    case EntityKind::LIGHT:
        if ( m_vLight.size() > index ) {
            retorno = m_vLight[index];
        }
        break;
    case EntityKind::OBJECT:
        if ( m_vObject.size() > index ) {
            retorno = m_vObject[index];
        }
        break;
    case EntityKind::PARTICLE_EMITTER:
        if ( m_vParticle.size() > index ) {
            retorno = m_vParticle[index];
        }
        break;
     case EntityKind::SKYBOX: //FIXME verificar como implementar
         if ( m_vSkyBox.size() > index ) {
             retorno = m_vSkyBox[index];
         }
         break;
    default:
        break;
    }

    return retorno;
}

void SceneMng::execLight() {
    for ( Light *pLight : m_vLight ) {
        pLight->apply( LIGHT0 );
    }
}

void SceneMng::init() {
 
    root->initScene();
    shadoMap.init ( (Node*)root );
    
}

void SceneMng::draw ( HUD *_pHud ) {

//#define TESTEZ1

    int indiceDesenho = 1;
    if ( pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS ) {
        indiceDesenho = 2;
    }

#ifdef TESTEZ1
    for ( Light *pLight : m_vLight ) {
        btVector3 posicao = pLight->getPosition();
        shadoMap.StoreLightMatrices ( posicao ); //FIXME so funciona para 1 luz
    }

    shadoMap.RenderSceneA ( pObjeto );
#endif

    for ( int eye = 0; eye < indiceDesenho; eye++ ) {

        pVideo->executeViewPerspective ( pCameraAtiva->getFov(),pCameraAtiva->getNear(),pCameraAtiva->getFar(), eye );

		pCameraAtiva->render();

        if ( pSkyBox != nullptr ) {
           pSkyBox->render ( true );
        }

        root->draw(pObjeto);
        
        execLight();

#ifdef TESTEZ1
        shadoMap.ApplyShadowMap ( pObjeto );
#endif

        if ( pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS ) {
            hudUpdate ( _pHud,0 );
        } else {
            hudUpdate ( _pHud,eye );
        }

    }

}

void SceneMng::hudUpdate ( HUD *_pHud,int eye ) {

    if ( _pHud->isOn() == true ) {
        pVideo->executeViewOrto ( eye );
        _pHud->update();
        pVideo->restoreMatrix();
    }

}

void SceneMng::setLight ( bool _lightOn ) {
    if ( _lightOn == true ) {
        glEnable ( GL_LIGHTING );
    } else {
        glDisable ( GL_LIGHTING );
    }
}

void SceneMng::setMaterial ( bool _materialOn ) {

    if (_materialOn == true) {
    	glDisable(GL_COLOR_MATERIAL);
    	glColorMaterial ( GL_FRONT, GL_DIFFUSE ); //TODO verificar necessidade
    }
    else {
    	glEnable(GL_COLOR_MATERIAL);
    	glColorMaterial(GL_FRONT, GL_DIFFUSE);  //TODO verificar necessidade
    }
}

} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
