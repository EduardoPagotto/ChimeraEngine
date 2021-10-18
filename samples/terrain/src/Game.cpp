#include "Game.hpp"
#include "chimera/core/CameraFPS.hpp"
//#include "chimera/core/CameraOrbit.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/LoadHeightMap.hpp"
#include "chimera/render/scene/CameraControllerFPS.hpp"
#include "chimera/render/scene/Components.hpp"
#include "chimera/render/scene/Entity.hpp"

Game::Game(Chimera::Canvas* canvas) : Application(canvas) {

    pShader = shaderLibrary.load("./assets/shaders/MeshFullShadow.glsl");

    // Light
    pLight = new Chimera::Light();
    pLight->setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    pLight->setAmbient(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
    pLight->setPosition(glm::vec3(0, 400, 0));

    pHeightMap = nullptr;
}

Game::~Game() { delete pShader; }

void Game::onStart() {

    using namespace Chimera;
    Entity ce = activeScene.createEntity("Camera Entity");
    { // Cria entidade de camera
        // Cria camera e carrega parametros
        CameraComponent& cc = ce.addComponent<CameraComponent>();
        cc.camera.setViewportSize(canvas->getWidth(), canvas->getHeight());
        cc.camera.setPosition(glm::vec3(0.0, 200.0, 0.0));

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

    Entity renderableEntity = activeScene.createEntity("Renderable Entity");

    Chimera::Material& material = renderableEntity.addComponent<Chimera::Material>();
    material.setDefaultEffect();
    material.setShine(50.0f);

    Chimera::TextureManager::loadFromFile("grid2", "./assets/textures/grid2.png", Chimera::TextureParameters());
    material.addTexture(SHADE_TEXTURE_DIFFUSE, Chimera::TextureManager::getLast());
    material.init();

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    // whitou the z-buffer
    // glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_NORMALIZE);
    // glShadeModel(GL_SMOOTH);

    // enable z-buffer here
    canvas->afterStart();

    Chimera::MeshData& mesh = renderableEntity.addComponent<Chimera::MeshData>();

    Chimera::LoadHeightMap loader(32, 32);
    loader.getMesh("./assets/heightmaps/terrain3.jpg", mesh, glm::vec3(1000.0, 200.0, 1000.0));
    // loader.getMesh("./assets/heightmaps/heightmap_16x16.png", mesh, glm::vec3(100.0, 30.0, 100.0));
    // loader.getMesh("./assets/heightmaps/heightmap_4x4.png", mesh, glm::vec3(1000.0, 10.0, 1000.0));
    loader.split(mesh.vertexIndex);

    std::vector<Chimera::VertexData> vertexDataIn;
    vertexDataFromMesh(&mesh, vertexDataIn);

    this->pHeightMap = new RenderableChunk(renderableEntity, loader.vNodes, vertexDataIn);

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
                    render3d.logToggle();
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
    return true;
}

void Game::onUpdate() {
    canvas->before();

    if (render3d.getLog() == true) {
        glm::vec3 pos = camera->getPosition();
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
    }

    pShader->enable();

    activeScene.onUpdate(0.01);
    // camera->processInput(0.01);
    // Calcula view e projection pela Camera
    glViewport(0, 0, canvas->getWidth(), canvas->getHeight());
    // camera->recalculateMatrix(canvas->getRatio());

    pLight->setUniform(pShader);

    // model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    // //_pMesh->getTransform()->getModelMatrix(pTransform->getPosition()); if (pShader == nullptr)
    //     return;

    int shadows = 0;
    pShader->setUniform("shadows", shadows);

    // pShader->setUniform("projection", camera->getProjectionMatrix());
    // pShader->setUniform("view", camera->getViewMatrix());
    // pShader->setUniform("model", model);

    // aplica material ao shader
    // pMaterial->bindMaterialInformation(pShader);

    // NEW
    render3d.begin(camera, pShader);
    pHeightMap->submit(&render3d); // render3d.submit(pHeightMap);
    render3d.end();

    render3d.flush(true);

    canvas->after();
    canvas->swapWindow();
}
