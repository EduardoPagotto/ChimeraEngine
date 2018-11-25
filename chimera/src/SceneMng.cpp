#include "SceneMng.hpp"
#include "InitVisitor.hpp"
#include "NodeParse.hpp"

namespace Chimera {

SceneMng::SceneMng() : Node(nullptr, EntityKind::SCENE_MANAGER, "DefaultSG") {
    log = spdlog::get("chimera");
    log->debug("Contructor SceneMng");
}

SceneMng::~SceneMng() {
    log->debug("Destructor SceneMng"); // TODO: deletar o grapho
}

void SceneMng::accept(NodeVisitor* v) { v->visit(this); }

void SceneMng::init() { log->debug("SceneMng Inicializado {0}", getName()); }

void SceneMng::start(Video* _pVideo) {

    _pVideo->initGL();

    InitVisitor initV;
    NodeParse::tree(this, &initV); // dfs(root, iv);

    _pVideo->afterStart();

    renderV.pVideo = _pVideo;
}

void SceneMng::draw(Video* _pVideo) {

    _pVideo->initDraw();

    for (int eye = 0; eye < _pVideo->getTotEyes(); eye++) {
        renderV.eye = eye;
        NodeParse::tree(this, &renderV); // dfs(root, &rv);//DFS(root);
    }

    _pVideo->endDraw();
}
} // namespace Chimera
