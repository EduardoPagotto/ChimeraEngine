#include "Game.hpp"
#include "chimera/core/CameraFPS.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/LoadObj.hpp"
#include "chimera/render/scene/CameraControllerFPS.hpp"
#include "chimera/render/scene/CameraControllerOrbit.hpp"
#include "chimera/render/scene/Components.hpp"
#include "chimera/render/scene/Entity.hpp"

Game::Game(Chimera::Canvas* canvas) : Application(canvas) { pShader = shaderLibrary.load("./assets/shaders/Simples.glsl"); }

Game::~Game() {}

void Game::onStart() {

    using namespace Chimera;
    Entity ce = activeScene.createEntity("Camera Entity");
    { // Cria entidade de camera
        // Cria camera e carrega parametros
        CameraComponent& cc = ce.addComponent<CameraComponent>();
        cc.camera.setViewportSize(canvas->getWidth(), canvas->getHeight());
        cc.camera.setPosition(glm::vec3(0.0f, 0.0f, 80.0f));

        this->camera = &cc.camera;

        // parametros de controller de camera (parametros DEFAULT!!!)
        auto cp = ce.addComponent<CameraControlerFPSParams>();
        cp.yaw = 0;
        cp.pitch = 0;

        // auto cp = ce.addComponent<CameraControlerOrbitParams>();
        // cp.yaw = 0;
        // cp.pitch = 0;

        // Adiciona um controller (Compostamento de FPS) a camera e vincula entidades ao mesmo
        ce.addComponent<NativeScriptComponent>().bind<CameraControllerFPS>("cameraFPS");
        // ce.addComponent<NativeScriptComponent>().bind<CameraControllerOrbit>("cameraOrbit");
    }

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color
    // Habilita o depth buffer/culling face
    canvas->afterStart();
    glEnable(GL_COLOR_MATERIAL);

    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    // glDisable(GL_LIGHTING);
    // glCullFace(GL_BACK);

    Entity renderableEntity = activeScene.createEntity("Renderable Entity");

    Chimera::MeshData& mesh = renderableEntity.addComponent<Chimera::MeshData>();
    Chimera::Material& material = renderableEntity.addComponent<Chimera::Material>();

    // Chimera::MeshData mesh;
    Chimera::LoaderObj loader;
    // loader.getMesh("./assets/models/tela01.obj", mesh);
    // loader.getMesh("./assets/models/square2.obj", mesh);
    // loader.getMesh("./assets/models/parede_simples.obj", mesh);
    // loader.getMesh("./assets/models/cubo_textura_simples.obj", mesh);
    loader.getMesh("./assets/models/map02.obj", mesh);
    // loader.getMesh("./assets/models/salaSplit3.obj", mesh); // Sala L com Split

    if (loader.hasMaterial() == true)
        loader.getMaterial(material);
    else {
        material.setDefaultEffect();
        Chimera::TextureManager::loadFromFile("tex_mapa", "./assets/textures/grid2.png", Chimera::TextureParameters());
        material.addTexture(SHADE_TEXTURE_DIFFUSE, Chimera::TextureManager::getLast());
    }

    material.init();

    this->renderable = new Chimera::RenderableSimple(renderableEntity);

    activeScene.onCreate();
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
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    Chimera::utilSendEvent(Chimera::EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_1:
                    render3D.logToggle();
                    break;
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

    // activeScene.onUpdate(0.01);

    return true;
}

void Game::onUpdate() {
    canvas->before();

    pShader->enable();

    activeScene.onUpdate(0.01);

    glViewport(0, 0, canvas->getWidth(), canvas->getHeight());
    // camControl->recalculateMatrix(false);

    render3D.begin(camera, pShader);
    renderable->submit(&render3D);
    render3D.end();

    render3D.flush(true);

    canvas->after();
    canvas->swapWindow();
}
