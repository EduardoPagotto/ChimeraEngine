#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/TrackHead.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/VisitParser.hpp"

Game::Game(Chimera::Core::CanvasGL* _pCanvas, Chimera::Node* _pRoot) : pCanvas(_pCanvas), pRoot(_pRoot) {}

Game::~Game() {}

void Game::joystickEvent(SDL_Event* pEventSDL) {}

void Game::keboardEvent(SDL_Event* pEventSDL) {

    Chimera::NodeCamera* pCamZ = (Chimera::NodeCamera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

    switch (pEventSDL->key.keysym.sym) {
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
}

void Game::mouseEvent(SDL_Event* pEventSDL) {

    Chimera::NodeCamera* pCamZ = (Chimera::NodeCamera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

    if (Chimera::Core::MouseDevice::getButtonState(1) == SDL_PRESSED) {
        if (pEventSDL->type == SDL_MOUSEMOTION) {
            pCamZ->getTrackBall()->tracking(pEventSDL->motion.xrel, pEventSDL->motion.yrel);
        }
    } else if (Chimera::Core::MouseDevice::getButtonState(3) == SDL_PRESSED) {

        if (pEventSDL->type == SDL_MOUSEMOTION) {
            pCamZ->getTrackBall()->offSet(pEventSDL->motion.yrel);
        }
    }
}

void Game::start() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    pRoot->initializeChilds();

    pCanvas->afterStart();

    Chimera::NodeMesh* pMesh = (Chimera::NodeMesh*)pRoot->findChild("Cubo-02", true);
    renderV.pTransform = pMesh->getTransform();
    renderV.pVideo = pCanvas;
}

void Game::userEvent(const SDL_Event& _event) {
    switch (_event.user.code) {
        case Chimera::Core::EVENT_TOGGLE_FULL_SCREEN:
            pCanvas->toggleFullScreen();
            break;
        case Chimera::Core::EVENT_FLOW_START:
            this->start();
        default:
            break;
    }
}

void Game::windowEvent(SDL_Event* pEventSDL) {
    switch (pEventSDL->window.event) {
        case SDL_WINDOWEVENT_ENTER:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pCanvas->reshape(pEventSDL->window.data1, pEventSDL->window.data2);
            break;
        default:
            break;
    }
}

void Game::update() {
    for (int eye = 0; eye < pCanvas->getTotEyes(); eye++) {

        pCanvas->before(eye);

        renderV.eye = eye;
        Chimera::visitParserTree(pRoot, &renderV); // dfs(root, &rv);//DFS(root);

        pCanvas->after(eye);
    }

    pCanvas->swapWindow();
}
