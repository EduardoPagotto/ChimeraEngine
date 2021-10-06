#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/TextureManager.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/render/LoadObj.hpp"
#include "chimera/render/maze/Maze.hpp"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::Core::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    Chimera::TextureManager::loadFromFile("tex01", "./data/images/grid2.png", Chimera::TextureParameters());
    pTex = Chimera::TextureManager::getLast();
}

Game::~Game() { delete renderz1; }

void Game::joystickEvent(SDL_Event* pEventSDL) {}

void Game::keboardEvent(SDL_Event* pEventSDL) {

    if (pEventSDL->type == SDL_KEYDOWN) {
        // Keyboard::isPressed(SDLK_ESCAPE)
        switch (pEventSDL->key.keysym.sym) {
            case SDLK_ESCAPE:
                Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_STOP, nullptr, nullptr);
                break;
            case SDLK_1:
                render3d.logToggle();
                break;
            case SDLK_F10:
                Chimera::Core::utilSendEvent(Chimera::Core::EVENT_TOGGLE_FULL_SCREEN, nullptr, nullptr);
                break;
            default:
                break;
        }
    }
}

void Game::mouseEvent(SDL_Event* pEventSDL) {

    if (Chimera::Core::MouseDevice::getButtonState(1) == SDL_PRESSED) {
        if (pEventSDL->type == SDL_MOUSEMOTION) {
            trackBall.tracking(pEventSDL->motion.xrel, pEventSDL->motion.yrel);
        }
    } else if (Chimera::Core::MouseDevice::getButtonState(3) == SDL_PRESSED) {

        if (pEventSDL->type == SDL_MOUSEMOTION) {
            trackBall.offSet(pEventSDL->motion.yrel);
        }
    }
}

void Game::start() {

    Chimera::ViewPoint* pVp = new Chimera::ViewPoint();
    pVp->position = glm::vec3(0.0, 60.0, 0.0);
    pVp->front = glm::vec3(0.0, 0.0, 0.0);
    pVp->up = glm::vec3(0.0, 1.0, 0.0);
    trackBall.init(pVp);
    trackBall.setMax(1000.0);

    glClearColor(0.f, 0.f, 0.f, 1.f); // Initialize clear color

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    // pTex->init();

    // Chimera::MeshData m;
    // Chimera::LoaderObj loader;
    // // loader.getMesh("./data/models/tela01.obj", m); // quadrado simples pequeno
    // // loader.getMesh("./data/models/salaSplit3.obj", m); // Sala L com Split apenas triangulos
    // loader.getMesh("./data/models/map02.obj", m); // Sala com 5 espacos em forma de X
    // // loader.getMesh("./data/models/parede_simples.obj", m); // FIXME Falha para EPSILON 1e-2
    // // loader.getMesh("./data/models/square2.obj", m); // 2 quadrado teste de split lateral
    // // loader.getMesh("./data/models/square1.obj", m); // 2 quadrado teste de split centro
    // // loader.getMesh("./data/models/split1.obj", m); // 2 triangulos sem textura
    // // loader.getMesh("./data/models/cubo_textura_simples.obj", m);
    // // m.changeSize(30.0, true);

    std::vector<uint32_t> vIndex;
    std::vector<Chimera::VertexData> vVertexIndexed;

    // Usando o Maze
    Chimera::Maze maze = Chimera::Maze("./data/maze7.txt");
    maze.createMap();

    vertexDataReorder(maze.vertexData, maze.vIndex, vVertexIndexed, vIndex);
    bspTree.create(vVertexIndexed, vIndex);

    renderz1 = new Chimera::RenderableBsp(bspTree.getRoot(), bspTree.getLeafs(), bspTree.getVertex());
}

void Game::userEvent(const SDL_Event& _event) {
    switch (_event.user.code) {
        case Chimera::Core::EVENT_TOGGLE_FULL_SCREEN:
            pCanvas->toggleFullScreen();
            break;
        case Chimera::Core::EVENT_FLOW_START:
            this->start();
        default:
            break;
    }
}

void Game::windowEvent(SDL_Event* pEventSDL) {
    switch (pEventSDL->window.event) {
        case SDL_WINDOWEVENT_ENTER:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_RESUME, nullptr, nullptr); // isPaused = false;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            Chimera::Core::utilSendEvent(Chimera::Core::EVENT_FLOW_PAUSE, nullptr, nullptr); // isPaused = true;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pCanvas->reshape(pEventSDL->window.data1, pEventSDL->window.data2);
            break;
        default:
            break;
    }
}

void Game::update() {
    pCanvas->before();

    Chimera::ViewPoint* vp = trackBall.getViewPoint();

    if (render3d.getLog() == true) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", vp->position.x, vp->position.y, vp->position.z);
    }

    pShader->enable();

    // Calcula view e projection baseado em vp
    pCanvas->calcPerspectiveProjectionView(0, vp, view, projection);

    glm::mat4 projectionMatrixInverse = glm::inverse(projection);
    glm::mat4 viewMatrixInverse = glm::inverse(view);
    glm::mat4 viewProjectionMatrixInverse = viewMatrixInverse * projectionMatrixInverse;
    frustum.set(viewProjectionMatrixInverse);

    pShader->setUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    // aplica a textura
    pTex->bind(0);
    pShader->setUniform1i(SHADE_TEXTURE_DIFFUSE, 0);

    renderz1->setEyePosition(&vp->position);
    render3d.begin(&frustum);
    renderz1->submit(&render3d); // render3d.submit(renderz1);
    render3d.end();

    render3d.flush();

    pCanvas->after();
    pCanvas->swapWindow();
}
