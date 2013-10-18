#include "SceneMng.h"

namespace Chimera {

SceneMng::SceneMng (Node *_pRoot) : pRoot(_pRoot) {
    pRoot = _pRoot;
    parseEntity(pRoot);
}

SceneMng::~SceneMng() {

}

void SceneMng::parseEntity(Node *_pNode) {
    
    addEntityToScene( _pNode );
    
    for (Node *node : _pNode->listChild) {
        parseEntity( node );
    }
    
}

void SceneMng::addEntityToScene( Node *_pNode) {

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
        case EntityKind::DRAW: 
            m_vDraw.push_back( ( Draw* ) _pNode );
            break;
        case EntityKind::IMAGE: 
            m_vImage.push_back( ( Image* ) _pNode );
            break;
        case EntityKind::MATERIAL: 
            m_vMaterial.push_back( ( Material* ) _pNode );
            break;
        case EntityKind::EFFECT: 
            m_vEffect.push_back( ( Effect* ) _pNode );
            break;
        default:
            break;
    }
    
}

void SceneMng::addChildToScene ( Node *_pNode ) {

    pRoot->addChild ( _pNode );
    addEntityToScene( _pNode );
    
}

Node *SceneMng::getNode ( EntityKind _type, int index ) {

    Node *retono = nullptr;

    switch ( _type ) {
    case EntityKind::CAMERA:
        if ( m_vCamera.size() > index )
            retono = m_vCamera[index];
        break;
    case EntityKind::LIGHT:
        if ( m_vLight.size() > index )
            retono = m_vLight[index];
        break;
    case EntityKind::OBJECT:
        if ( m_vObject.size() > index )
            retono = m_vObject[index];
        break;
    default:
        break;
    }

    return retono;
}

void SceneMng::execLight() {
    for ( Light *pLight : m_vLight ) {
        pLight->exec();
    }
}

void SceneMng::update(DataMsg *dataMsg ) {
    
    pRoot->update ( dataMsg );
        
}

} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
