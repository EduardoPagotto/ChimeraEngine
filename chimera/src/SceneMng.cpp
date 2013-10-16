#include "SceneMng.h"

namespace Chimera {

SceneMng::SceneMng () : Node(EntityKind::SCENE)  {

}

SceneMng::~SceneMng() {

}

void SceneMng::addChildToScene ( Node *_pNode ) {

    addChild ( _pNode );

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
    default:
        break;
    }

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
    
    Node::update ( dataMsg );
        
}

} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
