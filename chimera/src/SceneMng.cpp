#include "SceneMng.hpp"
#include "InitVisitor.hpp"
#include "NodeParse.hpp"

namespace Chimera {

SceneMng::SceneMng() : Node(nullptr, EntityKind::SCENE_MANAGER, "DefaultSG") {
    pOrigem = nullptr;

    log = spdlog::get("chimera");
    log->debug("Contructor SceneMng");

    pRV = new RenderVisitor();
}

SceneMng::~SceneMng() {
    // TODO: deletar o grapho

    log->debug("Destructor SceneMng");
    delete pRV;
    pRV = nullptr;
}

void SceneMng::accept(NodeVisitor* v) { v->visit(this); }

void SceneMng::init() { log->debug("SceneMng Inicializado {0}", getName()); }

void SceneMng::start(Video* _pVideo) {

    _pVideo->initGL();

    InitVisitor* iv = new InitVisitor();
    NodeParse::tree(this, iv); // dfs(root, iv);

    _pVideo->afterStart();
}

void SceneMng::draw(Video* _pVideo) {

    _pVideo->initDraw();

    int indiceDesenho = 1;
    // FIXME: colocar o HMD_Z1
    if (_pVideo->getKindDevice() == KIND_DEVICE::OVR_OCULUS) {
        indiceDesenho = 2;
    }

    pRV->pVideo = _pVideo;
    pRV->pCoord = pOrigem;
    for (int eye = 0; eye < indiceDesenho; eye++) {
        pRV->eye = eye;
        NodeParse::tree(this, pRV); // dfs(root, &rv);//DFS(root);
    }

    _pVideo->endDraw();
}
} // namespace Chimera
