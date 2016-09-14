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
}

void SceneMng::initNodes(Node* u, InitVisitor *pVisit)
{
    u->setColor(1);
    u->accept(pVisit);

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

void SceneMng::DFS(Node* u)
{
	u->setColor(1);
	u->accept(&rv);

	std::vector<Node*>* children = u->getChilds();
	if ((children != nullptr) && (children->size() >0)) {

		for (size_t i = 0; i < children->size(); i++) {
			Node* child = children->at(i);
			if (child->getColor() == 0)
				DFS(child);
		}
	}
	u->setColor(0);
}


void SceneMng::draw () {

    int indiceDesenho = 1;
    if ( pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS ) {
        indiceDesenho = 2;
    }

	rv.pVideo = pVideo;
    rv.pCoord = pOrigem;

    for ( int eye = 0; eye < indiceDesenho; eye++ ) {
        rv.eye = eye;
		DFS(root);
    }
}
} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
