#include "Game.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/render/3d/RenderableBsp.hpp"
#include "chimera/render/CameraOrbit.hpp"
#include "chimera/render/TextureManager.hpp"
#include "chimera/render/Transform.hpp"
#include "chimera/render/partition/BSPTree.hpp"
#include "chimera/render/partition/LoadObj.hpp"
#include "chimera/render/partition/Maze.hpp"
#include "chimera/render/scene/CameraController.hpp"
#include "chimera/render/scene/Components.hpp"

Game::Game(Chimera::Engine* engine) : engine(engine) {

    using namespace Chimera;
    Entity ce = activeScene.getRegistry().createEntity("Camera Entity");
    { // Cria entidade de camera
        // Cria camera e carrega parametros
        CameraComponent& cc = ce.addComponent<CameraComponent>();
        ce.addComponent<Transform>();
        cc.camera = new CameraOrbit(glm::vec3(0.0f, 0.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
        // cc.camera = new CameraFPS(glm::vec3(0.0f, 0.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

        ce.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
    }

    {
        Entity renderableEntity = activeScene.getRegistry().createEntity("Maze Entity");
        renderableEntity.addComponent<Transform>();
        Shader& shader = renderableEntity.addComponent<Shader>();
        Material& material = renderableEntity.addComponent<Material>();
        Renderable3dComponent& rc = renderableEntity.addComponent<Renderable3dComponent>();

        ShaderManager::load("./assets/shaders/MeshNoMat.glsl", shader);

        // material.setDefaultEffect();
        // material.setShine(50.0f);
        TextureManager::loadFromFile("grid2", "./assets/textures/grid2.png", TexParam());
        material.addTexture(SHADE_TEXTURE_DIFFUSE, TextureManager::getLast());
        material.init();

        std::vector<uint32_t> vIndex;
        std::vector<VertexData> vVertexIndexed;

        // Usando o Maze
        Maze maze = Maze("./assets/maps/maze7.txt");
        maze.createMap();

        vertexDataReorder(maze.vertexData, maze.vIndex, vVertexIndexed, vIndex);

        BspTree bspTree;
        bspTree.create(vVertexIndexed, vIndex);

        RenderableBsp* r = new RenderableBsp(renderableEntity, bspTree.getRoot(), bspTree.getLeafs(), bspTree.getVertex());
        rc.renderable = r;
    }

    {
        Entity renderableEntity = activeScene.getRegistry().createEntity("Zoltam Entity");
        Transform& tc = renderableEntity.addComponent<Transform>();
        tc.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        Material& material = renderableEntity.addComponent<Material>();
        Shader& shader = renderableEntity.addComponent<Shader>();
        ShaderManager::load("./assets/shaders/MeshNoMat.glsl", shader);

        MeshData& mesh = renderableEntity.addComponent<MeshData>();
        // ret = loadObjFile("./assets/models/tela01.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/salaSplit3.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/square2.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/parede_simples.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/cubo_textura_simples.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/map02.obj", &mesh, &material);
        loadObjFile("./assets/models/zoltanObj.obj", &mesh, &material);
        // ret = loadObjFile("./assets/models/cubo2.obj", &mesh, &material);
    }

    // mudar para o event
    activeScene.onViewportResize(engine->getCanvas()->getWidth(), engine->getCanvas()->getHeight());
    engine->pushState(&activeScene);
}

Game::~Game() {}

void Game::onAttach() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color
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
                    activeScene.getRender()->logToggle();
                    break;
                case SDLK_F10:
                    utilSendEvent(EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
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
