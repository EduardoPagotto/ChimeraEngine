#include "Game.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/AABB.hpp"
#include "chimera/render/LoadObj.hpp"
#include "chimera/render/maze/Maze.hpp"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {
    isPaused = false;
    debugParser = false;

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    pTex = new Chimera::TextureImg(SHADE_TEXTURE_DIFFUSE, "./data/images/grid2.png");
}

Game::~Game() { bspTree.destroy(); }

void Game::joystickEvent(Chimera::IO::JoystickState* pJoy, SDL_Event* pEventSDL) {}

void Game::keboardEvent(SDL_Keycode tecla) {

    switch (tecla) {
        case SDLK_ESCAPE:
            SDL_Event l_eventQuit;
            l_eventQuit.type = SDL_QUIT;
            if (SDL_PushEvent(&l_eventQuit) == -1) {
                throw Chimera::Exception(std::string(SDL_GetError()));
            }
            break;
        case SDLK_1:
            debugParser = true;
            break;
        case SDLK_F10:
            Chimera::IO::utilSendEvent(Chimera::IO::TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseEvent(Chimera::IO::MouseDevice* pMouse, SDL_Event* pEventSDL) {

    if (pMouse->getButtonState(1) == SDL_PRESSED) {
        if (pEventSDL->type == SDL_MOUSEMOTION) {
            trackBall.tracking(pEventSDL->motion.xrel, pEventSDL->motion.yrel);
        }
    } else if (pMouse->getButtonState(3) == SDL_PRESSED) {

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

    pCanvas->initGL();

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    pTex->init();

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

    // // Cria VertexList sequencial
    // std::vector<Chimera::VertexData> vVertexSequencial;
    // m.toVertexData(vVertexSequencial);
    std::vector<uint32_t> vIndex;

    // // -------
    // // Comentar as tres linhas de baixo para bsp sem indice (sequencial)
    // // Cria BSP usando Vertex indexado
    std::vector<Chimera::VertexData> vVertexIndexed;
    // vertexDataIndexCompile(vVertexSequencial, vVertexIndexed, vIndex);
    // bspTree.create(vVertexIndexed, vIndex);

    // -------
    // Cria o BSP usando um vertes sequencial sem indice
    // bspTree.create(true, vVertexSequencial, vIndex);
    // -------

    // Usando o Maze
    Chimera::Maze maze = Chimera::Maze("./data/maze7.txt");
    maze.createMap();

    vertexDataReorder(maze.vertexData, maze.vIndex, vVertexIndexed, vIndex);

    renderz1 = bspTree.create(vVertexIndexed, vIndex);
}

void Game::stop() {}

void Game::newFPS(const unsigned int& fps) {}

void Game::userEvent(const SDL_Event& _event) {
    if (_event.user.code == Chimera::IO::TOGGLE_FULL_SCREEN) {
        pCanvas->toggleFullScreen();
    }
}

void Game::windowEvent(const SDL_WindowEvent& _event) {
    switch (_event.event) {
        case SDL_WINDOWEVENT_ENTER:
            isPaused = false;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            isPaused = true;
            break;
        case SDL_WINDOWEVENT_RESIZED:
            pCanvas->reshape(_event.data1, _event.data2);
            break;
    }
}

bool Game::paused() { return isPaused; }

void Game::render() {
    pCanvas->before();

    Chimera::ViewPoint* vp = trackBall.getViewPoint();

    if (debugParser == true) {
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
    pTex->apply(pShader);

    bspTree.render(&vp->position, frustum, debugParser);

    // TO debug only
    bspTree.renderAABB();

    pCanvas->after();
    pCanvas->swapWindow();
}
