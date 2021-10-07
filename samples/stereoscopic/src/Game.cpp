#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/TrackHead.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/VisitParser.hpp"

Game::Game(Chimera::Core::CanvasGL* _pCanvas, Chimera::Node* _pRoot) : pCanvas(_pCanvas), pRoot(_pRoot) {}

Game::~Game() {}

void Game::onStart() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    pRoot->initializeChilds();

    pCanvas->afterStart();

    Chimera::NodeMesh* pMesh = (Chimera::NodeMesh*)pRoot->findChild("Cubo-02", true);
    renderV.pTransform = pMesh->getTransform();
    renderV.pVideo = pCanvas;
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;
    using namespace Chimera::Core;
    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case Chimera::Core::EVENT_TOGGLE_FULL_SCREEN:
                    pCanvas->toggleFullScreen();
                    break;
            }

        } break;
        case SDL_KEYDOWN: {
            Chimera::NodeCamera* pCamZ = (Chimera::NodeCamera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_a:
                    pCamZ->getTrackWalk()->move(Chimera::Camera_Movement::LEFT, 10);
                    break;
                case SDLK_d:
                    pCamZ->getTrackWalk()->move(Chimera::Camera_Movement::RIGHT, 10);
                    break;
                // case SDLK_1:
                //     render3D.logToggle();
                case SDLK_F10:
                    Chimera::Core::utilSendEvent(Chimera::Core::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
                default:
                    break;
            }
        } break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION: {
            Chimera::NodeCamera* pCamZ = (Chimera::NodeCamera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

            if (Chimera::Core::MouseDevice::getButtonState(1) == SDL_PRESSED) {
                if (event.type == SDL_MOUSEMOTION) {
                    pCamZ->getTrackBall()->tracking(event.motion.xrel, event.motion.yrel);
                }
            } else if (Chimera::Core::MouseDevice::getButtonState(3) == SDL_PRESSED) {

                if (event.type == SDL_MOUSEMOTION) {
                    pCamZ->getTrackBall()->offSet(event.motion.yrel);
                }
            }
        } break;
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    pCanvas->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate() {
    for (int eye = 0; eye < pCanvas->getTotEyes(); eye++) {

        pCanvas->before(eye);

        renderV.eye = eye;
        Chimera::visitParserTree(pRoot, &renderV); // dfs(root, &rv);//DFS(root);

        pCanvas->after(eye);
    }

    pCanvas->swapWindow();
}
