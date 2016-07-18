#include "SceneMng.h"

namespace Chimera {

SceneMng::SceneMng ( Video *_pVideo ) : pVideo(_pVideo) {

    root = new SceneRoot(nullptr,"root");
    pCameraAtiva = nullptr;
	pOrigemDesenho = nullptr;
    pLoader = nullptr;
}

SceneMng::~SceneMng() {
}

void SceneMng::setReader ( LoaderDae* _pLoader ) {

    this->pLoader = _pLoader;
}

Group *SceneMng::createSceneGraph() {
   
	Group *pGroup = nullptr;
	if (pLoader!= nullptr)
		pGroup = pLoader->getNodes();
	else
		pGroup = new Group(nullptr, "Default");

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
    default:
        break;
    }

    return retorno;
}

void SceneMng::init() {
 
    root->init();
    shadoMap.init ( (Node*)root );
}

void SceneMng::draw () {

//#define TESTEZ1

    int indiceDesenho = 1;
    if ( pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS ) {
        indiceDesenho = 2;
    }

#ifdef TESTEZ1
	btVector3 posicao = root->getState()->getLight()->getPosition();
	shadoMap.StoreLightMatrices(posicao); //FIXME so funciona para 1 luz
    shadoMap.RenderSceneA (pOrigemDesenho);
#endif

    for ( int eye = 0; eye < indiceDesenho; eye++ ) {

		if (pCameraAtiva != nullptr) {

			pVideo->executeViewPerspective(pCameraAtiva->getFov(), pCameraAtiva->getNear(), pCameraAtiva->getFar(), eye);
			pCameraAtiva->render();
		}

        root->draw(pOrigemDesenho);
        
#ifdef TESTEZ1
        shadoMap.ApplyShadowMap (pOrigemDesenho);
#endif
        if ( pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS ) {
            hudUpdate ( 0 );
        } else {
            hudUpdate ( eye );
        }
    }
}

void SceneMng::hudUpdate ( int eye ) {

    pVideo->executeViewOrto ( eye );

	root->drawHud(nullptr);

    pVideo->restoreMatrix();

}

//void SceneMng::setLight ( bool _lightOn ) {
//    if ( _lightOn == true ) {
//        glEnable ( GL_LIGHTING );
//    } else {
//        glDisable ( GL_LIGHTING );
//    }
//}
//
//void SceneMng::setMaterial ( bool _materialOn ) {
//
//    if (_materialOn == true) {
//    	glDisable(GL_COLOR_MATERIAL);
//    	glColorMaterial ( GL_FRONT, GL_DIFFUSE ); //TODO verificar necessidade
//    }
//    else {
//    	glEnable(GL_COLOR_MATERIAL);
//    	glColorMaterial(GL_FRONT, GL_DIFFUSE);  //TODO verificar necessidade
//    }
//}
} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
