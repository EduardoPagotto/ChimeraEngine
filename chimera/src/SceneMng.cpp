#include "SceneMng.h"

namespace Chimera {

SceneMng::SceneMng ( Engine3D *_pEngine3D ) : m_pEngine3D ( _pEngine3D ) {
    m_pRoot = new Node ( EntityType::NODE,std::string ( "root" ),std::string ( "root" ) );
}

SceneMng::~SceneMng() {

}

void SceneMng::addChildToScene ( Node *_pNode ) {

    m_pRoot->addChild ( _pNode );

    switch ( _pNode->getType() ) {
    case EntityType::CAMERA:
        m_vCamera.push_back ( ( Camera* ) _pNode );
        break;
    case EntityType::LIGHT:
        m_vLight.push_back ( ( Light* ) _pNode );
        break;
    case EntityType::OBJECT:
        m_vObject.push_back ( ( Object* ) _pNode );
        break;
    default:
        break;
    }

}

Node *SceneMng::getNode ( EntityType _type, int index ) {

    Node *retono = nullptr;

    switch ( _type ) {
    case EntityType::CAMERA:
        if ( m_vCamera.size() > index )
            retono = m_vCamera[index];
        break;
    case EntityType::LIGHT:
        if ( m_vLight.size() > index )
            retono = m_vLight[index];
        break;
    case EntityType::OBJECT:
        if ( m_vObject.size() > index )
            retono = m_vObject[index];
        break;
    default:
        break;
    }

    return retono;
}

void SceneMng::execute ( DataMsg *dataMsg ) {

    m_pRoot->update ( dataMsg );

    if ( m_pEngine3D->getLight() ==true ) {
        for ( Light *pLight : m_vLight ) {
            pLight->exec();
        }
    }

}


} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
