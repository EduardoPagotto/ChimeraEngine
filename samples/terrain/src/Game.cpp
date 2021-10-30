#include "Game.hpp"
#include "chimera/core/CameraFPS.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/3d/RenderableChunk.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/partition/LoadHeightMap.hpp"
#include "chimera/render/partition/LoadObj.hpp"
#include "chimera/render/scene/CameraController.hpp"
#include "chimera/render/scene/Components.hpp"

Game::Game(Chimera::Canvas* canvas) : Application(canvas) {}

Game::~Game() {}

void Game::onStart() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color //FIXME: colocar so scene

    // whitou the z-buffer
    // glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_NORMALIZE);
    // glShadeModel(GL_SMOOTH);

    // enable z-buffer here
    canvas->afterStart();

    using namespace Chimera;

    {
        // Cria entidade de camera
        Entity ce = activeScene.createEntity("Camera Entity");
        CameraComponent& cc = ce.addComponent<CameraComponent>();
        // cc.camera = new CameraOrbit(glm::vec3(0.0, 200.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
        cc.camera = new CameraFPS(glm::vec3(20.0, 200.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

        // Adiciona um controller (Compostamento) a camera e vincula entidades ao mesmo
        ce.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
    }
    {
        // Cria entidade de luz unica global!
        Entity le = activeScene.createEntity("Light Entity");
        LightComponent& lc = le.addComponent<LightComponent>();
        Light* light = new Light();
        light->setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        light->setAmbient(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
        light->setPosition(glm::vec3(0, 400, 0));
        lc.light = light;
    }
    {
        // entidade heightmap
        Entity renderableEntity = activeScene.createEntity("Renderable Entity");

        Shader& shader = renderableEntity.addComponent<Shader>();
        Material& material = renderableEntity.addComponent<Material>();
        Renderable3dComponent& rc = renderableEntity.addComponent<Renderable3dComponent>();

        // ShaderManager::load("./assets/shaders/MeshNoMat.glsl", shader);
        ShaderManager::load("./assets/shaders/MeshFullShadow.glsl", shader);

        material.setDefaultEffect(); // FIXME: removido para evitar msg de erro, ja que shader nao tem variavel!!!
        material.setShine(50.0f);

        TextureManager::loadFromFile("grid2", "./assets/textures/grid2.png", TexParam());
        material.addTexture(SHADE_TEXTURE_DIFFUSE, TextureManager::getLast());
        material.init();

        MeshData mesh;

        LoadHeightMap loader(32, 32);
        loader.getMesh("./assets/heightmaps/terrain3.jpg", mesh, glm::vec3(1000.0, 200.0, 1000.0));
        // loader.getMesh("./assets/heightmaps/heightmap_16x16.png", mesh, glm::vec3(100.0, 30.0, 100.0));
        // loader.getMesh("./assets/heightmaps/heightmap_4x4.png", mesh, glm::vec3(1000.0, 10.0, 1000.0));
        loader.split(mesh.vertexIndex);

        std::vector<VertexData> vertexDataIn;
        vertexDataFromMesh(&mesh, vertexDataIn);

        RenderableChunk* r = new RenderableChunk(renderableEntity, loader.vNodes, vertexDataIn);
        rc.renderable = r;
    }

    {
        // Entidade mesh
        Entity renderableEntity = activeScene.createEntity("Renderable Entity");

        // Transform& tc = renderableEntity.getComponent<Transform>();
        // tc.setPosition(glm::vec3(0.0f, 200.0f, 0.0f));

        Material& material = renderableEntity.addComponent<Material>();
        Shader& shader = renderableEntity.addComponent<Shader>();
        ShaderManager::load("./assets/shaders/MeshNoMat.glsl", shader); // colocar shader em material

        MeshData mesh;
        int ret = 0;
        // ret = loadObjFile("./assets/models/tela01.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/salaSplit3.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/square2.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/parede_simples.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/cubo_textura_simples.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/map02.obj", &mesh, &material);
        ret = loadObjFile("./assets/models/zoltanObj.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/cubo2.obj", &mesh, &material);

        std::vector<Chimera::VertexData> renderData;
        vertexDataFromMesh(&mesh, renderData);

        std::vector<uint32_t> index;
        std::vector<Chimera::VertexData> vertexDataOut;
        vertexDataIndexCompile(renderData, vertexDataOut, index);

        Renderable3dComponent& rc = renderableEntity.addComponent<Renderable3dComponent>();
        RenderableSimple* r = new RenderableSimple();
        r->createBuffers(&vertexDataOut[0], vertexDataOut.size(), &index[0], index.size());
        r->setEntity(renderableEntity);
        rc.renderable = r;

        material.init();
    }

    activeScene.onViewportResize(canvas->getWidth(), canvas->getHeight());
    activeScene.onCreate();
}

bool Game::onEvent(const SDL_Event& event) {
    using namespace Chimera;

    switch (event.type) {
        case SDL_USEREVENT: {
            switch (event.user.code) {
                case EVENT_TOGGLE_FULL_SCREEN:
                    canvas->toggleFullScreen();
                    break;
            }

        } break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    utilSendEvent(EVENT_FLOW_STOP, nullptr, nullptr);
                    break;
                case SDLK_1:
                    render3d.logToggle();
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
                    canvas->reshape(event.window.data1, event.window.data2);
                    break;
            }
        } break;
    }
    return true;
}

void Game::onUpdate() {
    canvas->before();

    // model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    // //_pMesh->getTransform()->getModelMatrix(pTransform->getPosition()); if (pShader == nullptr)
    //     return;
    activeScene.render(render3d);

    canvas->after();
    canvas->swapWindow();
}
