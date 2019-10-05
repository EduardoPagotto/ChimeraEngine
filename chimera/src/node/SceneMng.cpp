#include "chimera/node/SceneMng.hpp"
#include "chimera/node/InitVisitor.hpp"
#include "chimera/node/NodeParse.hpp"

namespace Chimera {

SceneMng::SceneMng() : Node(nullptr, EntityKind::SCENE_MANAGER, "DefaultSG") {}

SceneMng::~SceneMng() {}

void SceneMng::accept(NodeVisitor* v) { v->visit(this); }

void SceneMng::init() {}

void SceneMng::start(CanvasGL* _pVideo) {

    _pVideo->initGL();

    InitVisitor initV;
    NodeParse::tree(this, &initV); // dfs(root, iv);

    _pVideo->afterStart();

    renderV.pVideo = _pVideo;
}

void SceneMng::draw(CanvasGL* _pVideo) {

    for (int eye = 0; eye < _pVideo->getTotEyes(); eye++) {

        _pVideo->before(eye);

        renderV.eye = eye;
        NodeParse::tree(this, &renderV); // dfs(root, &rv);//DFS(root);

        _pVideo->after(eye);
    }

    _pVideo->swapWindow();
}
} // namespace Chimera
