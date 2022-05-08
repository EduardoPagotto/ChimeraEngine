#include "Game.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/core/visible/CameraFPS.hpp"
#include "chimera/core/visible/Components.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/TextureManager.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/3d/RenderableArray.hpp"
#include "chimera/render/partition/LoadHeightMap.hpp"
#include "chimera/render/partition/LoadObj.hpp"
#include "chimera/render/scene/CameraController.hpp"
#include "chimera/render/scene/Components.hpp"

Game::Game(Chimera::Engine* engine) : engine(engine) {
    using namespace Chimera;
    {
        // Cria entidade de camera
        Entity ce = activeScene.getRegistry().createEntity("Camera Entity");
        TransComponent& tc = ce.addComponent<TransComponent>();
        tc.trans = new Transform();

        ComponentCamera& cc = ce.addComponent<ComponentCamera>();
        // cc.camera = new CameraOrbit(glm::vec3(0.0, 200.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
        cc.camera = new CameraFPS(glm::vec3(20.0, 200.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
        cam = (ICamera3D*)cc.camera;

        // Adiciona um controller (Compostamento) a camera e vincula entidades ao mesmo
        ce.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
    }
    {
        // Cria entidade de luz unica global!
        Entity le = activeScene.getRegistry().createEntity("Light Entity");
        TransComponent& tc = le.addComponent<TransComponent>();
        tc.trans = new Transform();
        tc.trans->setPosition(glm::vec3(0, 400, 0));

        LightComponent& lc = le.addComponent<LightComponent>();
        Light* light = new Light();
        light->setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        light->setAmbient(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
        lc.light = light;
    }
    {
        // entidade heightmap
        Entity renderableEntity = activeScene.getRegistry().createEntity("Heightmap Entity");

        TransComponent& tc = renderableEntity.addComponent<TransComponent>();
        tc.trans = new Transform();

        Shader& shader = renderableEntity.addComponent<Shader>();
        MaterialComponent& material = renderableEntity.addComponent<MaterialComponent>();
        Renderable3dComponent& rc = renderableEntity.addComponent<Renderable3dComponent>();

        std::unordered_map<GLenum, std::string> shadeData;
        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/MeshFullShadow.frag";
        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/MeshFullShadow.vert";
        ShaderManager::load("MeshFullShadow", shadeData, shader);

        material.material->setDefaultEffect(); // FIXME: removido para evitar msg de erro, ja que shader nao tem variavel!!!
        material.material->setShine(50.0f);

        material.material->addTexture(SHADE_TEXTURE_DIFFUSE,
                                      TextureManager::loadFromFile("grid2", "./assets/textures/grid2.png", TexParam()));
        material.material->init();

        Mesh mesh;

        LoadHeightMap loader(32, 32);
        loader.getMesh("./assets/heightmaps/terrain3.jpg", mesh, glm::vec3(1000.0, 200.0, 1000.0));
        // loader.getMesh("./assets/heightmaps/heightmap_16x16.png", mesh, glm::vec3(100.0, 30.0, 100.0));
        // loader.getMesh("./assets/heightmaps/heightmap_4x4.png", mesh, glm::vec3(1000.0, 10.0, 1000.0));
        loader.split(mesh.iPoint);

        std::vector<VertexData> vertexDataIn;
        vertexDataFromMesh(&mesh, vertexDataIn);

        RenderableArray* r = new RenderableArray(loader.vNodes, vertexDataIn);
        rc.renderable = r;
    }
    {
        // Entidade mesh
        Entity renderableEntity = activeScene.getRegistry().createEntity("Zoltam Entity");
        TransComponent& tc = renderableEntity.addComponent<TransComponent>();
        tc.trans = new Transform();
        tc.trans->setPosition(glm::vec3(0.0f, 200.0f, 0.0f));

        MaterialComponent& material = renderableEntity.addComponent<MaterialComponent>();
        material.material->setDefaultEffect(); // FIXME: removido para evitar msg de erro, ja que shader nao tem variavel!!!
        material.material->setShine(50.0f);

        Shader& shader = renderableEntity.addComponent<Shader>();
        std::unordered_map<GLenum, std::string> shadeData;
        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/MeshFullShadow.frag";
        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/MeshFullShadow.vert";
        ShaderManager::load("MeshFullShadow", shadeData, shader); // colocar shader em material

        MeshComponent& mesh = renderableEntity.addComponent<MeshComponent>();
        loadObjFile("./assets/models/cubo2.obj", mesh.mesh, material.material);
    }
    activeScene.onViewportResize(engine->getCanvas()->getWidth(), engine->getCanvas()->getHeight());
    engine->pushState(&activeScene);
}

Game::~Game() {}

void Game::onAttach() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color //FIXME: colocar so scene
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
            }
        } break;
    }
    return true;
}

void Game::onUpdate(const double& ts) {
    glm::vec3 pos = cam->getPosition();
    glm::vec3 front = cam->getFront();
    glm::vec3 up = cam->getUp();

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "pos(%.2f; %.2f; %.2f)", pos.x, pos.y, pos.z);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "front(%.2f; %.2f; %.2f)", front.x, front.y, front.z);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "up(%.2f; %.2f; %.2f)", up.x, up.y, up.z);
}

void Game::onRender() {}