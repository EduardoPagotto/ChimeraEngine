#include "Game.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/partition/BSPTree.hpp"
#include "chimera/core/partition/Maze.hpp"
#include "chimera/core/partition/Wavefront.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/core/visible/CameraOrbit.hpp"
#include "chimera/core/visible/TextureManager.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/3d/RenderableBsp.hpp"
#include "chimera/render/scene/CameraController.hpp"
#include "chimera/render/scene/Components.hpp"

Game::Game(Chimera::Scene* scene, Chimera::Engine* engine) : scene(scene) {

    using namespace Chimera;
    Entity ce = scene->getRegistry().createEntity("Camera Entity");

    {
        // injeta controlador de camera
        auto view1 = scene->getRegistry().get().view<CameraComponent>();
        for (auto entity : view1) {
            Entity e = Entity{entity, &scene->getRegistry()};
            e.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
        }
    }

    {
        Entity renderableEntity = scene->getRegistry().createEntity("Maze Entity");
        TransComponent& tc = renderableEntity.addComponent<TransComponent>();
        tc.trans = new Transform();

        Shader& shader = renderableEntity.addComponent<Shader>();
        MaterialComponent& material = renderableEntity.addComponent<MaterialComponent>();
        Renderable3dComponent& rc = renderableEntity.addComponent<Renderable3dComponent>();

        std::unordered_map<GLenum, std::string> shadeData;
        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/MeshNoMat.frag";
        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/MeshNoMat.vert";
        ShaderManager::load("MeshNoMat", shadeData, shader);

        Mesh mesh;

        // std::string matFile;
        // wavefrontObjLoad("./assets/models/map02.obj", &mesh, matFile);
        // if (matFile.size() > 0) {
        //     wavefrontMtlLoad(matFile, material.material);
        // }

        material.material->addTexture(SHADE_TEXTURE_DIFFUSE,
                                      TextureManager::loadFromFile("grid2", "./assets/textures/grid2.png", TexParam()));
        material.material->init();

        // processa o Maze
        Maze maze = Maze("./assets/maps/maze7.txt", &mesh);
        maze.createMap();

        Mesh mesh2;
        meshReCompile(mesh, mesh2);

        // btree root, leafs, vertex
        BspTree bspTree;
        std::vector<TrisIndex> vTrisFinal;
        BSPTreeNode* root = bspTree.create(&mesh2, vTrisFinal);
        RenderableBsp* r = new RenderableBsp(root, vTrisFinal, &mesh2);
        rc.renderable = r;
    }

    engine->pushState(this);
    engine->pushState(scene);
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
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    utilSendEvent(EVENT_FLOW_STOP, nullptr, nullptr);
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
            }
        } break;
    }
    return true;
}

void Game::onUpdate(const double& ts) {}

void Game::onRender() {}
