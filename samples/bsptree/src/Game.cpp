#include "Game.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/core/visible/CameraOrbit.hpp"
#include "chimera/core/visible/TextureManager.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/3d/RenderableBsp.hpp"
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
        TransComponent& tc = ce.addComponent<TransComponent>();
        tc.trans = new Transform();

        cc.camera = new CameraOrbit(glm::vec3(0.0f, 0.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
        // cc.camera = new CameraFPS(glm::vec3(0.0f, 0.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

        ce.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
    }

    {
        Entity renderableEntity = activeScene.getRegistry().createEntity("Maze Entity");
        TransComponent& tc = renderableEntity.addComponent<TransComponent>();
        tc.trans = new Transform();

        Shader& shader = renderableEntity.addComponent<Shader>();
        Material& material = renderableEntity.addComponent<Material>();
        Renderable3dComponent& rc = renderableEntity.addComponent<Renderable3dComponent>();

        std::unordered_map<GLenum, std::string> shadeData;
        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/MeshNoMat.frag";
        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/MeshNoMat.vert";
        ShaderManager::load("MeshNoMat", shadeData, shader);

        // material.setDefaultEffect();
        // material.setShine(50.0f);
        TextureManager::loadFromFile("grid2", "./assets/textures/grid2.png", TexParam());
        material.addTexture(SHADE_TEXTURE_DIFFUSE, TextureManager::getLast());
        material.init();

        // processa o Maze
        Maze maze = Maze("./assets/maps/maze7.txt");
        maze.createMap();

        // resultado da compressao do maze
        std::vector<uint32_t> vIndex;
        std::vector<VertexData> vVertexIndexed;
        vertexDataReorder(maze.vertexData, maze.vIndex, vVertexIndexed, vIndex);

        // indexador triangular
        std::list<Triangle*> vTris;
        if (vIndex.size() > 0)
            vertexDataIndexToTriangle(&vVertexIndexed[0], &vIndex[0], vIndex.size(), vTris);
        else
            vertexDataToTriangle(&vVertexIndexed[0], vVertexIndexed.size(), vTris);

        // btree root, leafs, vertex
        BspTree bspTree;
        std::vector<VertexData> vVertexFinal;
        VecPrtTrisIndex vTrisFinal;
        BSPTreeNode* root = bspTree.create(vVertexIndexed, vTris, vVertexFinal, vTrisFinal);

        RenderableBsp* r = new RenderableBsp(root, vTrisFinal, vVertexFinal);
        rc.renderable = r;
    }

    {
        Entity renderableEntity = activeScene.getRegistry().createEntity("Zoltam Entity");
        TransComponent& tc = renderableEntity.addComponent<TransComponent>();
        tc.trans = new Transform();
        tc.trans->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        Material& material = renderableEntity.addComponent<Material>();
        Shader& shader = renderableEntity.addComponent<Shader>();

        std::unordered_map<GLenum, std::string> shadeData;
        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/MeshNoMat.frag";
        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/MeshNoMat.vert";
        ShaderManager::load("MeshNoMat", shadeData, shader);

        Mesh& mesh = renderableEntity.addComponent<Mesh>();
        loadObjFile("./assets/models/cubo2.obj", &mesh, &material);
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
                    activeScene.logToggle();
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
