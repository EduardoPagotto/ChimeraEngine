#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/render/LoadHeightMap.hpp"
#include <glm/gtc/type_ptr.hpp>

Game::Game() {
    isPaused = false;
    debugParser = false;

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    pCanvas = new Chimera::CanvasGL("Chimera", 400, 300);

    pShader = new Chimera::Shader("Simples1", Chimera::shadeLoadProg("MeshNoMat", "./chimera/shaders/MeshNoMat.vert",
                                                                     "./chimera/shaders/MeshNoMat.frag"));

    pMaterial = new Chimera::Material();
    pMaterial->setDefaultEffect();
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
    pLight->setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    pLight->setPosition(glm::vec3(80, 100, 150));

    pHeightMap = nullptr;
}

Game::~Game() {
    delete pShader;
    delete pCanvas;
}

void Game::joystickCapture(Chimera::JoystickManager& joy) {}

void Game::joystickStatus(Chimera::JoystickManager& joy) {}

void Game::keyCapture(SDL_Keycode tecla) {

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
            Chimera::eventsSend(Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN, nullptr, nullptr);
            break;
        default:
            break;
    }
}

void Game::mouseButtonUpCapture(SDL_MouseButtonEvent mb) {
    botaoIndex = mb.button;
    estadoBotao = mb.state;
}

void Game::mouseButtonDownCapture(SDL_MouseButtonEvent mb) {
    botaoIndex = mb.button;
    estadoBotao = mb.state;
}

void Game::mouseMotionCapture(SDL_MouseMotionEvent mm) {

    if (estadoBotao == SDL_PRESSED) {
        if (botaoIndex == 1)
            trackBall.tracking(mm.xrel, mm.yrel);
        else if (botaoIndex == 2)
            trackBall.offSet(mm.yrel);
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

    Chimera::LoadHeightMap loader;
    loader.getMesh("./data/terrain/heightmap_16x16.png", m);

    m.changeSize(100.0f, true);

    pHeightMap = new Chimera::HeightMap(loader.getWidth(), loader.getHeight(), 4, 4);
    pHeightMap->split(m);

    // TODO: aqui ele usa o index completo, mudar para o node
    std::vector<Chimera::VertexData> vertexDataIn;
    m.toVertexData(vertexDataIn);

    // so teste
    // std::vector<Chimera::Triangle> vecTriangle;
    // std::vector<unsigned int> index;
    // m.toTriangle(vecTriangle, index);

    pHeightMap->createVertexBuffer(vertexDataIn);
}

void Game::stop() {}

void Game::newFPS(const unsigned int& fps) {}

void Game::userEvent(const SDL_Event& _event) {
    Chimera::KindOp op = (Chimera::KindOp)_event.user.code;
    if (op == Chimera::KindOp::VIDEO_TOGGLE_FULL_SCREEN) {
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
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", vp->position.x, vp->position.y,
                     vp->position.z);
    }

    pShader->link();

    // Calcula view e projection baseado em vp
    pCanvas->calcPerspectiveProjectionView(0, vp, view, projection);

    pLight->apply(pShader);

    model = glm::translate(
        glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0)); //_pMesh->getTransform()->getModelMatrix(pTransform->getPosition());
    if (pShader == nullptr)
        return;

    // int shadows = 1;
    // pShader->setGlUniform1i("shadows", shadows);
    pShader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    // aplica material ao shader
    pMaterial->apply(pShader);

    // NEW
    pHeightMap->render();

    pCanvas->after();
    pCanvas->swapWindow();
}
