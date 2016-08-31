#include "SceneMng.h"

namespace Chimera {

SceneMng::SceneMng ( Video *_pVideo ) : pVideo(_pVideo) {

    root = new SceneRoot(nullptr,"root");
    pCameraAtiva = nullptr;
	pOrigem = nullptr;
    pLoader = nullptr;
}

SceneMng::~SceneMng() {
}

void SceneMng::setReader ( LoaderDae* _pLoader ) {

    this->pLoader = _pLoader;
}

Group *SceneMng::createSceneGraph() {
   
	Group *pGroup = nullptr;
	if (pLoader != nullptr)
		pGroup = pLoader->getNodes();
	else
		pGroup = new Group(nullptr, "Default");

    root->addChild(pGroup);
    
    return pGroup;
}

void SceneMng::init() {
 
    InitVisitor *iv = new InitVisitor();
    initNodes(root, iv);
    
   // shadoMap.init ( (Node*)root );

}


void SceneMng::initNodes(Node* u, InitVisitor *pVisit)
{
    u->setColor(1);
    u->accept(pVisit); //entrypoint
    
    std::vector<Node*>* children = u->getChilds();
    if ((children != nullptr)&&(children->size() > 0)) {
        
        for (size_t i = 0; i < children->size(); i++) {
            
            Node* child = children->at(i);
            if(child->getColor() == 0)
				initNodes(child, pVisit);
        }
    }
    u->setColor(0);
}

//void SceneMng::DFS(Node* u)
//{
//    u->setColor(1);
//   // glPushMatrix();
//    u->accept(&rv);
//    
//    std::vector<Node*>* children = u->getChilds();
//    if ((children!=nullptr) && (children->size() >0)){
//        
//        for (size_t i = 0; i < children->size(); i++) {
//            Node* child = children->at(i);
//            if(child->getColor()==0)
//				DFS(child);
//        }
//    }
//    u->setColor(0);
//  //  glPopMatrix();
//}

void SceneMng::draw () {

//#define TESTEZ1

    int indiceDesenho = 1;
    if ( pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS ) {
        indiceDesenho = 2;
    }

	rv.pVideo = pVideo;
    rv.pCoord = pOrigem;
    
//#ifdef TESTEZ1
//    
//    //rv.textureOn = false;
//    rv.HudOn = false;
//    rv.particleOn = false;
//	rv.runningShadow = true;
//
//	Camera *nodeCam = (Camera*)root->findChild(Chimera::EntityKind::CAMERA, 0, true);
//	glm::vec3 posicao = nodeCam->getPosition();//root->findChild(Chimera::EntityKind::CAMERA, 0, true);//getState()->getLight()->getPosition();
//	rv.lightSpaceMatrix = shadoMap.lightSpaceMatrix;
//	shadoMap.StoreLightMatrices(posicao); //FIXME so funciona para 1 luz
//	shadoMap.initSceneShadow();
//	DFS(root);
//	shadoMap.endSceneShadow();
//    //shadoMap.RenderSceneA (pOrigemDesenho);
//#endif
   
    for ( int eye = 0; eye < indiceDesenho; eye++ ) {

		//rv.HudOn = true;
		//rv.particleOn = true;
		//rv.eye = eye;
 		//DFS(root);
		rv.execute(root, eye);

//#ifdef TESTEZ1
//        //shadoMap.ApplyShadowMap (pOrigemDesenho);
//		rv.HudOn = false;
//		rv.particleOn = false;
//		shadoMap.initApplyShadow();
//		DFS(root);
//		shadoMap.endApplyShadow();
//#endif
    }
}
} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
