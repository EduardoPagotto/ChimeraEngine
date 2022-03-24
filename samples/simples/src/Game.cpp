#include "Game.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/core/visible/CameraOrbit.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/render/partition/LoadObj.hpp"
#include "chimera/render/scene/CameraController.hpp"
#include "chimera/render/scene/Components.hpp"

Game::Game(Chimera::Engine* engine) : engine(engine) {
    using namespace Chimera;
    {
        Entity ce = activeScene.getRegistry().createEntity("Camera Entity");
        TransComponent& tc = ce.addComponent<TransComponent>();
        tc.trans = new Transform();

        // Cria entidade de camera
        // Cria camera e carrega parametros
        CameraComponent& cc = ce.addComponent<CameraComponent>();
        cc.camera = new CameraOrbit(glm::vec3(0.0f, 0.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
        // cc.camera = new CameraFPS(glm::vec3(0.0f, 0.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

        // Adiciona um controller (Compostamento) a camera e vincula entidades ao mesmo
        ce.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
    }

    {
        Entity renderableEntity = activeScene.getRegistry().createEntity("Renderable Entity");
        TransComponent& tc = renderableEntity.addComponent<TransComponent>();
        tc.trans = new Transform();

        MaterialComponent& material = renderableEntity.addComponent<MaterialComponent>();
        Shader& shader = renderableEntity.addComponent<Shader>();

        std::unordered_map<GLenum, std::string> shadeData;
        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/MeshNoMat.frag";
        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/MeshNoMat.vert";
        ShaderManager::load("MeshNoMat", shadeData, shader);

        Mesh& mesh = renderableEntity.addComponent<Mesh>();

        int ret = 0;
        ret = loadObjFile("./assets/models/cubo2.obj", &mesh, material.material);
    }

    activeScene.onViewportResize(engine->getCanvas()->getWidth(), engine->getCanvas()->getHeight());
    engine->pushState(&activeScene);
}

Game::~Game() {}

void Game::onAttach() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color
    // Habilita o depth buffer/culling face
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onDeatach() {}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case EVENT_TOGGLE_FULL_SCREEN:
                    engine->getCanvas()->toggleFullScreen();
                    break;
            }

        } break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    utilSendEvent(EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_1:
                    activeScene.logToggle();
                    break;
                case SDLK_F10:
                    utilSendEvent(EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
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
                    utilSendEvent(EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    utilSendEvent(EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    engine->getCanvas()->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }

    return true;
}

void Game::onUpdate(const double& ts) {}

void Game::onRender() {}
