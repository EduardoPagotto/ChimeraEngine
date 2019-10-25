#include "chimera/node/SceneMng.hpp"
#include "chimera/node/InitVisitor.hpp"
#include "chimera/node/NodeParse.hpp"

namespace Chimera {

SceneMng::SceneMng(CanvasGL* _pCanvas, Node* _pRoot) : pCanvas(_pCanvas), pRoot(_pRoot) {}

SceneMng::~SceneMng() {}

void SceneMng::init() {

    pCanvas->initGL();

    InitVisitor initV;
    NodeParse::tree(pRoot, &initV); // dfs(root, iv);

    pCanvas->afterStart();

    renderV.pVideo = pCanvas;
}

void SceneMng::render() {

    for (int eye = 0; eye < pCanvas->getTotEyes(); eye++) {

        pCanvas->before(eye);

        renderV.eye = eye;
        NodeParse::tree(pRoot, &renderV); // dfs(root, &rv);//DFS(root);

        pCanvas->after(eye);
    }

    pCanvas->swapWindow();
}
} // namespace Chimera
