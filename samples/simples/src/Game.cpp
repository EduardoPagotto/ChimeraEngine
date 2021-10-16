#include "Game.hpp"
#include "chimera/core/CameraFPS.hpp"
//#include "chimera/core/CameraOrbit.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/LoadObj.hpp"
#include "chimera/render/scene/CameraControllerFPS.hpp"
#include "chimera/render/scene/Components.hpp"
#include "chimera/render/scene/Entity.hpp"

Game::Game(Chimera::Canvas* canvas) : Application(canvas) {

    pShader = shaderLibrary.load("./assets/shaders/Simples.glsl");
    model = glm::mat4(1.0f);
}

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

        // Adiciona um controller (Compostamento de FPS) a camera e vincula entidades ao mesmo
        ce.addComponent<NativeScriptComponent>().bind<CameraControllerFPS>("cameraFPS");
        auto nsc = ce.getComponent<NativeScriptComponent>();
        nsc.instance = nsc.instantiateScript();
        nsc.instance->setEntity(Entity{ce, &activeScene});

        // camera = new Chimera::CameraFPS(glm::vec3(-80.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), 0.0, 0.0);
        // camera = new Chimera::CameraOrbit(glm::vec3(-80.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), 0.0, 0.0);
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

    Chimera::MeshData m;
    Chimera::LoaderObj loader;
    // loader.getMesh("./assets/models/tela01.obj", m);
    // loader.getMesh("./assets/models/square2.obj", m);
    // loader.getMesh("./assets/models/parede_simples.obj", m);
    // loader.getMesh("./assets/models/cubo_textura_simples.obj", m);
    loader.getMesh("./assets/models/map02.obj", m);
    // loader.getMesh("./assets/models/salaSplit3.obj", m); // Sala L com Split

    if (loader.hasMaterial() == true)
        loader.getMaterial(material);
    else {
        material.setDefaultEffect();
        Chimera::TextureManager::loadFromFile("tex_mapa", "./assets/textures/grid2.png", Chimera::TextureParameters());
        material.addTexture(SHADE_TEXTURE_DIFFUSE, Chimera::TextureManager::getLast());
    }

    material.init();

    // m.changeSize(100.0, material.hasTexture());
    // m.textureFix();

    std::vector<Chimera::VertexData> renderData;
    vertexDataFromMesh(&m, renderData);

    std::vector<uint32_t> index;
    std::vector<Chimera::VertexData> vertexDataOut;
    vertexDataIndexCompile(renderData, vertexDataOut, index);
    // this->rederable = new Chimera::RenderableStatic(&vertexDataOut[0], vertexDataOut.size(), &index[0], index.size());
    this->renderable = new Chimera::RenderableSimple(&vertexDataOut[0], vertexDataOut.size(), &index[0], index.size());

    activeScene.onCreate();

    // auto zz = ce.getComponent<NativeScriptComponent>();
    // camControl = (CameraControllerFPS*)&zz.instance;
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
    // camera->processInput(0.01); // FIXME: ver onde fica melhor

    glViewport(0, 0, canvas->getWidth(), canvas->getHeight());
    // camControl->recalculateMatrix(false);
    // camera->recalculateMatrix(canvas->getRatio());

    pShader->setUniform("projection", camera->getProjectionMatrix());
    pShader->setUniform("view", camera->getViewMatrix());
    pShader->setUniform("model", model);

    material.bindMaterialInformation(pShader);

    render3D.begin(camera);
    renderable->submit(&render3D);
    render3D.end();

    render3D.flush();

    canvas->after();
    canvas->swapWindow();
}
