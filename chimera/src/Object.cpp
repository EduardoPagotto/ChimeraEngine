#include "Object.h"

namespace Chimera {

Object::Object ( std::string _id, std::string _name ) : Node ( EntityKind::OBJECT,_id,_name ) {

    pPhysic = nullptr;
    pDraw = nullptr;
    pTexture = nullptr;
    pEffect = nullptr;

    collide = false;

    transform.setIdentity();

}

Object::Object ( const Object& _object ) : Node ( _object ) {

    pPhysic = _object.pPhysic;
    pDraw = _object.pDraw;
    pTexture = _object.pTexture;
    pEffect = _object.pEffect;
    collide = _object.collide;

    transform = _object.transform;

}

Object::~Object () {
}

void Object::setPositionRotation ( btVector3 _posicao, btVector3 _rotation ) {

    //Transformacao quando Euley nao apagar
    btQuaternion l_qtn;
    transform.setIdentity();
    l_qtn.setEulerZYX ( _rotation.getX(), _rotation.getY(), _rotation.getZ() );
    transform.setRotation ( l_qtn );
    transform.setOrigin ( _posicao );
    //pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), l_posicao));

}

void Object::init() {

    pTexture = ( Texture* ) findChildByKind ( EntityKind::TEXTURE,0 );
    pEffect = ( Effect* ) findChildByKind ( EntityKind::EFFECT,0 );
    pDraw = ( Draw* ) findChildByKind ( EntityKind::DRAW,0 );
    pPhysic = ( Physics* ) findChildByKind ( EntityKind::PHYSICS,0 );

    if ( pEffect == nullptr ) {

        pEffect = new Effect ( "effect_interno","effect_interno" );
        pEffect->setDiffuse ( Color ( 0.6, 0.6, 0.6 ) );
        pEffect->setEmissive ( Color ( 0.1,0.1,0.1 ) );
        pEffect->setAmbient ( Color ( 0.1,0.1,0.1 ) );
        pEffect->setSpecular ( Color ( 0.5,0.5,0.5 ) );
        pEffect->setShininess ( 0.5 );

    }

}

void Object::render() {

    if ( pEffect != nullptr )
        pEffect->render();

    if ( pTexture != nullptr )
        pTexture->render();

    if ( pDraw!=nullptr )
        pDraw->render();

}

void Object::clone ( Node **ppNode ) {
    *ppNode = new Object ( *this );
    Node::clone ( ppNode );
}

void Object::update ( DataMsg *_dataMsg ) {

    if ( _dataMsg->getKindOp() ==KindOp::START ) {

        init();

        if ( pPhysic == nullptr ) {

            //FIXME apenas para teste define propriedades fisicas
            pPhysic = new Physics ( "","" );
            pPhysic->setMass ( 5.0f );
            pPhysic->setFriction ( 0.0f );
            pPhysic->setRestitution ( 0.0f );

        }

        if ( pDraw != nullptr )
            pPhysic->setShapeBox ( pDraw->getSizeBox() );

        pPhysic->init ( transform , this );

        Node::update ( _dataMsg );

    }
    if ( _dataMsg->getKindOp() == KindOp::DRAW3D ) {

        glPushMatrix();

        if ( pPhysic!=nullptr ) {
            Object *pSource = ( Object * ) _dataMsg->getParam();
            pPhysic->ajusteMatrix ( pSource->pPhysic );
        }

        render();

        Node::update ( _dataMsg );

        glPopMatrix();
    }
    if ( _dataMsg->getKindOp() == KindOp::IS_ALLOW_COLLIDE ) {

        _dataMsg->setDone ( true );

    }
}

btVector3& Object::getPosition() {
    return pPhysic->getPosition();
}

void Object::applyTorc ( const btVector3 &_vet ) {
    if ( pPhysic )
        pPhysic->torque ( _vet );
}

void Object::applyForce ( const btVector3 &_vet ) {
    if ( pPhysic )
        pPhysic->propulcao ( _vet );
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
