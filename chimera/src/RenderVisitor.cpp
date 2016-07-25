#include "RenderVisitor.h"

namespace Chimera {
   
RenderVisitor::RenderVisitor() {

}
  
RenderVisitor::~RenderVisitor() {

}

void RenderVisitor::visit ( Camera* _pCamera ) {

}

void RenderVisitor::visit ( Draw* _pDraw ) {
    
    _pDraw->renderExecute(textureOn);

}

void RenderVisitor::visit ( Light* _pLight ) {

}

void RenderVisitor::visit ( ParticleEmitter* _pParticleEmitter ) {
    
    _pParticleEmitter->l_vec = pOrigemDesenho->getPosition();
    _pParticleEmitter->renderExecute(true);

}

void RenderVisitor::visit ( SceneRoot* _pSceneRoot ) {
    
    _pSceneRoot->apply();

}

void RenderVisitor::visit ( Group* _pGroup ) {

}

void RenderVisitor::visit ( Chimera::Transform* _pTransform) {
    
    _pTransform->applyTransform();
    
}

void RenderVisitor::visit ( Solid* _pSolid ) {
    
    _pSolid->apply(pOrigemDesenho);

}

void RenderVisitor::visit ( Chimera::Coord* ) {

}

void RenderVisitor::visit ( HUD* _pHUD ) {

    pVideo->executeViewOrto ( eye );
    _pHUD->renderExecute(textureOn);
    pVideo->restoreMatrix(); 
    
}

}