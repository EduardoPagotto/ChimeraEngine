#include "SceneMng.h"
#include "NodeParse.h"
#include "InitVisitor.h"

namespace Chimera {

SceneMng::SceneMng () : Node(nullptr, EntityKind::SCENE_MANAGER, "DefaultSG") {
	pOrigem = nullptr;
}

SceneMng::~SceneMng() {
	//TODO: deletar o grapho
}

void SceneMng::accept(NodeVisitor* v){
    v->visit(this);
}

void SceneMng::init() {
    InitVisitor *iv = new InitVisitor();
    NodeParse::tree(this, iv); //dfs(root, iv);
}

void SceneMng::draw (Video *_pVideo) {

    int indiceDesenho = 1;
    // FIXME: colocar o HMD_Z1
    if ( _pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS ) {
        indiceDesenho = 2;
    }

    rv.pVideo = _pVideo;
    rv.pCoord = pOrigem;
    for ( int eye = 0; eye < indiceDesenho; eye++ ) {
        rv.eye = eye;
		NodeParse::tree(this, &rv); //dfs(root, &rv);//DFS(root);
    }
}
} /* namespace Chimera */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
