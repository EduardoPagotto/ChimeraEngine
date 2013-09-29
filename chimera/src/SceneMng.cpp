#include "SceneMng.h"

namespace Chimera {

SceneMng::SceneMng () {
    m_pRoot = new Node ( EntityType::NODE,std::string ( "root" ) );
}

SceneMng::~SceneMng() {

}

void SceneMng::addChildToScene ( Node *_pNode ) {

    m_pRoot->addChild ( _pNode );

    switch ( _pNode->getType() ) {
    case EntityType::CAMERA:
        m_lCamera.push_back ( ( Camera* ) _pNode );
        break;
    case EntityType::LIGHT:
        m_lLight.push_back ( ( Light* ) _pNode );
        break;
    case EntityType::OBJECT:
        m_lObject.push_back ( ( Object* ) _pNode );
        break;
    default:
        break;
    }

}

} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
