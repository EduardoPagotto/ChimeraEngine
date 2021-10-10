#include "Game.hpp"
//#include "chimera/core/CameraFPS.hpp"
#include "chimera/core/CameraOrbit.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeLight.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/VisitParser.hpp"

Game::Game(Chimera::Canvas* canvas, const std::vector<std::string>& shadesFile) : Application(canvas) {

    using namespace Chimera;

    Shader* pShader = nullptr;
    for (auto shaderFile : shadesFile)
        pShader = shaderLibrary.load(shaderFile); // so tem 1 mesmo!!!

    NodeGroup* groupRoot = new NodeGroup(nullptr, "modelos");
    groupRoot->setShader(pShader);

    // Propriedades da camera
    NodeCamera* pCam = new Chimera::NodeCamera(groupRoot, "Observador-01");
    pCam->setCamera(new CameraOrbit(glm::vec3(0, 0, 300), glm::vec3(0.0, 1.0, 0.0), 0.0, 0.0));

    // Propriedades da luz
    NodeLight* pLight = new Chimera::NodeLight(groupRoot, "Luz1");
    pLight->data.setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    pLight->data.setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    pLight->data.setPosition(glm::vec3(80, 100, 150));

    createMeshTexturizade(groupRoot, "Cubo-01", "./assets/models/cubo_textura_simples.obj", 5.0, glm::vec3(0.0, 0.0, 0.0));
    createMeshTexturizade(groupRoot, "Cubo-02", "./assets/models/cubo2.obj", 10.0, glm::vec3(150.0, 0.0, 0.0));

    pRoot = groupRoot;
}

Game::~Game() {}

void Game::onStart() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    pRoot->initializeChilds();

    canvas->afterStart();

    Chimera::NodeMesh* pMesh = (Chimera::NodeMesh*)pRoot->findChild("Cubo-02", true);
    renderV.pTransform = pMesh->getTransform();
    renderV.pVideo = (Chimera::CanvasGL*)canvas;
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case Chimera::EVENT_TOGGLE_FULL_SCREEN:
                    canvas->toggleFullScreen();
                    break;
            }

        } break;
        case SDL_KEYDOWN: {
            Chimera::NodeCamera* pCamZ = (Chimera::NodeCamera*)pRoot->findChild(Chimera::Kind::CAMERA, 0, true);

            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_a:
                    // pCamZ->getTrackWalk()->move(Chimera::Camera_Movement::LEFT, 10);
                    break;
                case SDLK_d:
                    // pCamZ->getTrackWalk()->move(Chimera::Camera_Movement::RIGHT, 10);
                    break;
                // case SDLK_1:
                //     render3D.logToggle();
                case SDLK_F10:
                    Chimera::utilSendEvent(Chimera::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                    break;
                default:
                    break;
            }
        } break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION: {
        } break;
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_ENTER:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    canvas->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate() {
    for (int eye = 0; eye < canvas->getTotEyes(); eye++) {

        canvas->before(eye);

        renderV.eye = eye;
        Chimera::visitParserTree(pRoot, &renderV); // dfs(root, &rv);//DFS(root);

        canvas->after(eye);
    }

    canvas->swapWindow();
}
