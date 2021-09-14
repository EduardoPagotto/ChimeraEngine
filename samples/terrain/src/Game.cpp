#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/utils.hpp"
#include "chimera/render/LoadHeightMap.hpp"
#include <glm/gtc/type_ptr.hpp>

Game::Game() {
    isPaused = false;
    debugParser = false;

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    pCanvas = new Chimera::CanvasGL("Chimera", 600, 400);
    pShader = new Chimera::Shader("MeshFullShadow", "./chimera/shaders/MeshFullShadow.vert", "./chimera/shaders/MeshFullShadow.frag");

    pMaterial = new Chimera::Material();
    pMaterial->setDefaultEffect();
    pMaterial->setShine(50.0f);
    pMaterial->addTexture(new Chimera::TextureImg(SHADE_TEXTURE_DIFFUSE, "./data/images/grid2.png"));

    Chimera::ViewPoint* pVp = new Chimera::ViewPoint();
    pVp->position = glm::vec3(0.0, 0.0, 600.0);
    pVp->front = glm::vec3(0.0, 0.0, 0.0);
    pVp->up = glm::vec3(0.0, 1.0, 0.0);
    trackBall.init(pVp);
    trackBall.setMax(1000.0);

    // Light
    pLight = new Chimera::Light();
    pLight->setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    pLight->setAmbient(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
    pLight->setPosition(glm::vec3(0, 150, 0));

    pHeightMap = nullptr;
}

Game::~Game() {
    delete pShader;
    delete pCanvas;
}

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

    pCanvas->initGL();

    // whitou the z-buffer
    // glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_NORMALIZE);
    // glShadeModel(GL_SMOOTH);

    // enable z-buffer here
    pCanvas->afterStart();

    // pTex->init();
    pMaterial->init();

    Chimera::MeshData m;

    Chimera::LoadHeightMap loader(32, 32);
    loader.getMesh("./data/terrain/terrain3.jpg", m, glm::vec3(1000.0, 200.0, 1000.0));
    // loader.getMesh("./data/terrain/heightmap_16x16.png", m, glm::vec3(100.0, 30.0, 100.0));
    // loader.getMesh("./data/terrain/heightmap_4x4.png", m, glm::vec3(1000.0, 10.0, 1000.0));
    loader.split(m.vertexIndex);

    std::vector<Chimera::VertexData> vertexDataIn;
    vertexDataFromMesh(&m, vertexDataIn);

    pHeightMap = new Chimera::HeightMap(loader.vNodes, vertexDataIn);
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

    pLight->apply(pShader);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0)); //_pMesh->getTransform()->getModelMatrix(pTransform->getPosition());
    if (pShader == nullptr)
        return;

    int shadows = 0;
    pShader->setUniform1i("shadows", shadows);

    pShader->setUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    // aplica material ao shader
    pMaterial->apply(pShader);

    // NEW
    render3d.begin(&vp->position, &frustum, debugParser);
    render3d.submit(pHeightMap);
    render3d.flush();
    render3d.end();

    pCanvas->after();
    pCanvas->swapWindow();
}
