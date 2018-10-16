#include "SceneMng.h"
#include "NodeParse.h"
#include "InitVisitor.h"

namespace Chimera {

SceneMng::SceneMng ( Video *_pVideo ) : pVideo(_pVideo) {

    root = new SceneRoot(nullptr,"root");
    pCameraAtiva = nullptr;
	pOrigem = nullptr;
    pLoader = nullptr;
}

SceneMng::~SceneMng() {
	//TODO: deletar o grapho
}

void SceneMng::setReader ( LoaderDae* _pLoader ) {

    this->pLoader = _pLoader;
}

Group *SceneMng::createSceneGraph() {

	Group *pGroup = nullptr;
	if (pLoader != nullptr)
		pGroup = pLoader->getNodes();
	else
		pGroup = new Group(nullptr, "DefaultSG");

    root->addChild(pGroup);

    return pGroup;
}

void SceneMng::init() {

    InitVisitor *iv = new InitVisitor();
    NodeParse::tree(root,iv); //dfs(root, iv);
}

void SceneMng::draw () {

    int indiceDesenho = 1;
    // FIXME: colocar o HMD_Z1
    if ( pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS ) {
        indiceDesenho = 2;
    }

	rv.pVideo = pVideo;
    rv.pCoord = pOrigem;

    for ( int eye = 0; eye < indiceDesenho; eye++ ) {
        rv.eye = eye;
		NodeParse::tree(root, &rv); //dfs(root, &rv);//DFS(root);
    }
}
} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
