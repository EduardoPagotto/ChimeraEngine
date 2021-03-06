#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/utils.hpp"
#include "chimera/render/LoadObj.hpp"
#include <glm/gtc/type_ptr.hpp>

Game::Game(Chimera::CanvasGL* _pCanvas, Chimera::Shader* _pShader) : pCanvas(_pCanvas), pShader(_pShader) {
    isPaused = false;
    debug_init = 0;
    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
}

Game::~Game() {}

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
            debug_init = 1;
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "iniciado log");
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
        else if (botaoIndex == 3)
            trackBall.offSet(mm.yrel);
    }
}

void Game::start() {

    Chimera::ViewPoint* pVp = new Chimera::ViewPoint();
    pVp->position = glm::vec3(0.0, 0.0, 600.0);
    pVp->front = glm::vec3(0.0, 0.0, 0.0);
    pVp->up = glm::vec3(0.0, 1.0, 0.0);
    trackBall.init(pVp);
    trackBall.setMax(1000.0);

    pCanvas->initGL();
    // Habilita o depth buffer/culling face
    pCanvas->afterStart();
    glEnable(GL_COLOR_MATERIAL);

    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    // glDisable(GL_LIGHTING);
    // glCullFace(GL_BACK);

    Chimera::MeshData m;
    Chimera::LoaderObj loader;
    // loader.getMesh("./data/models/tela01.obj", m);
    // loader.getMesh("./data/models/square2.obj", m);
    // loader.getMesh("./data/models/parede_simples.obj", m);
    // loader.getMesh("./data/models/cubo_textura_simples.obj", m);
    // loader.getMesh("./data/models/map02.obj", m);
    loader.getMesh("./data/models/salaSplit3.obj", m); // Sala L com Split

    if (loader.hasMaterial() == true)
        loader.getMaterial(material);
    else {
        material.setDefaultEffect();
        material.addTexture(new Chimera::TextureImg(SHADE_TEXTURE_DIFFUSE, "./data/images/grid2.png"));
    }

    material.init();

    // m.changeSize(100.0, material.hasTexture());
    // m.textureFix();

    std::vector<Chimera::VertexData> renderData;
    m.toVertexData(renderData);

    std::vector<unsigned int> index;
    vertexRenderStatic.create(renderData, index);
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

    pShader->link();

    // Calcula view e projection baseado em vp
    pCanvas->calcPerspectiveProjectionView(0, vp, view, projection);

    pShader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    material.apply(pShader);
    // aplica a textura
    // pTex->apply(0, "material.tDiffuse", pShader);

    vertexRenderStatic.render();

    pCanvas->after();
    pCanvas->swapWindow();
}
