#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/render/partition/LoadObj.hpp"
#include "chimera/render/partition/Maze.hpp"
#include "chimera/render/scene/CameraControllerFPS.hpp"
#include "chimera/render/scene/CameraControllerOrbit.hpp"
#include "chimera/render/scene/Components.hpp"
#include "chimera/render/scene/Entity.hpp"
#include <algorithm>

Game::Game(Chimera::Canvas* canvas) : Application(canvas) {}
Game::~Game() {}

void Game::onStart() {

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    using namespace Chimera;
    Entity ce = activeScene.createEntity("Camera Entity");
    { // Cria entidade de camera
        // Cria camera e carrega parametros
        CameraComponent& cc = ce.addComponent<CameraComponent>();
        cc.camera.setViewportSize(canvas->getWidth(), canvas->getHeight());
        cc.camera.setPosition(glm::vec3(0.0f, 0.0f, 80.0f));

        this->camera = &cc.camera;

        // parametros de controller de camera (parametros DEFAULT!!!)
        // auto cp = ce.addComponent<CameraControlerFPSParams>();
        // cp.yaw = 0;
        // cp.pitch = 0;

        auto cp = ce.addComponent<CameraControlerOrbitParams>();
        cp.yaw = 0;
        cp.pitch = 0;

        // Adiciona um controller (Compostamento de FPS) a camera e vincula entidades ao mesmo
        // ce.addComponent<NativeScriptComponent>().bind<CameraControllerFPS>("cameraFPS");
        ce.addComponent<NativeScriptComponent>().bind<CameraControllerOrbit>("cameraOrbit");

        activeScene.setCamera(&cc.camera);
    }

    Entity renderableEntity = activeScene.createEntity("Renderable Entity");
    Shader& shader = renderableEntity.addComponent<Shader>();
    Material& material = renderableEntity.addComponent<Material>();
    Renderable3dComponent& rc = renderableEntity.addComponent<Renderable3dComponent>();

    ShaderManager::load("./assets/shaders/MeshNoMat.glsl", shader);

    // material.setDefaultEffect();
    // material.setShine(50.0f);
    TextureManager::loadFromFile("grid2", "./assets/textures/grid2.png", TextureParameters());
    material.addTexture(SHADE_TEXTURE_DIFFUSE, TextureManager::getLast());
    material.init();

    std::vector<uint32_t> vIndex;
    std::vector<VertexData> vVertexIndexed;

    // Usando o Maze
    Maze maze = Maze("./assets/maps/maze7.txt");
    maze.createMap();

    vertexDataReorder(maze.vertexData, maze.vIndex, vVertexIndexed, vIndex);
    bspTree.create(vVertexIndexed, vIndex);

    RenderableBsp* r = new RenderableBsp(renderableEntity, bspTree.getRoot(), bspTree.getLeafs(), bspTree.getVertex());
    rc.renderable = r;

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

    glViewport(0, 0, canvas->getWidth(), canvas->getHeight()); // FIXME: ver se da para irar de todos!!!!
                                                               // camera->recalculateMatrix(canvas->getRatio());

    activeScene.render(render3d);

    canvas->after();
    canvas->swapWindow();
}
